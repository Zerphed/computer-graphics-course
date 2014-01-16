#define _USE_MATH_DEFINES

#include "App.hpp"
#include "base/Main.hpp"
#include "gpu/GLContext.hpp"
#include "io/File.hpp"
#include "io/StateDump.hpp"
#include "base/Random.hpp"
#include "gui/Image.hpp"
#include "extra.h"
#include "surf.h"
#include "parse.h"

#include <stdio.h>
#include <conio.h>

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/GLU.h>

using namespace FW;
using namespace std;

//------------------------------------------------------------------------

App::App(void)
:   m_action        (Action_None),
    m_commonCtrl    (CommonControls::Feature_Default & ~CommonControls::Feature_RepaintOnF5),
    m_cullMode      (CullMode_None),

    m_mousePressed (false),
    m_curveMode     (CurveMode_Draw),
    m_surfaceMode   (SurfaceMode_Draw),
    m_pointMode     (PointMode_True),

    m_lineLen      (0.1f)
{
    m_commonCtrl.showFPS(true);

    m_commonCtrl.addButton((S32*)&m_action, Action_LoadSWP,     FW_KEY_L,       "Load SWP... (L)");
    m_commonCtrl.addButton((S32*)&m_action, Action_ResetView,   FW_KEY_SPACE,   "Reset view... (SPACE)");
    m_commonCtrl.addButton((S32*)&m_action, Action_WriteOBJ,    FW_KEY_O,       "Write to .obj (O)");
    m_commonCtrl.addSeparator();

    m_commonCtrl.addToggle((S32*)&m_curveMode, CurveMode_None,     FW_KEY_NONE,    "Don't draw curve");
    m_commonCtrl.addToggle((S32*)&m_curveMode, CurveMode_Draw,    FW_KEY_NONE,    "Draw curve");
    m_commonCtrl.addToggle((S32*)&m_curveMode, CurveMode_Normals,  FW_KEY_NONE,    "Draw normals");    
    m_commonCtrl.addSeparator();

    m_commonCtrl.addToggle((S32*)&m_surfaceMode, SurfaceMode_None,      FW_KEY_NONE,    "Don't draw surface");
    m_commonCtrl.addToggle((S32*)&m_surfaceMode, SurfaceMode_Draw,      FW_KEY_NONE,    "Draw surface");
    m_commonCtrl.addToggle((S32*)&m_surfaceMode, SurfaceMode_WireFrame, FW_KEY_NONE,    "Draw wireframe");    
    m_commonCtrl.addSeparator();

    m_commonCtrl.addToggle((S32*)&m_pointMode, PointMode_False, FW_KEY_NONE,    "Don't draw control points");
    m_commonCtrl.addToggle((S32*)&m_pointMode, PointMode_True,  FW_KEY_NONE,    "Draw control points");    
    m_commonCtrl.addSeparator();

    m_window.setTitle("basis");
    m_window.addListener(this);
    m_window.addListener(&m_commonCtrl);

    m_window.getGL()->swapBuffers();

    initRendering();

    m_camera.SetDimensions(600, 600);
    m_camera.SetDistance(10);
    m_camera.SetCenter(Vec3f(0, 0, 0));
}

//------------------------------------------------------------------------

App::~App(void)
{}

//------------------------------------------------------------------------

bool App::handleEvent(const Window::Event& ev)
{
    if (ev.type == Window::EventType_Close)
    {
        m_window.showModalMessage("Exiting...");
        delete this;
        return true;
    }

    if (ev.type == Window::EventType_KeyDown)
    {
        keyDownFunc(ev);
    }

    if (ev.type == Window::EventType_KeyUp)
    {
        keyUpFunc(ev);
    }

    if (ev.type == Window::EventType_Mouse)
    {
        motionFunc(ev);
    }

    Action action  = m_action;
    m_action = Action_None;
    String name;
    Mat4f  eye;
    eye.setIdentity();

    switch (action)
    {
    case Action_None:
        break;

    case Action_LoadSWP:
        name = m_window.showFileLoadDialog("Load");
        if (name.getLength())
            loadObjects(name.getPtr());
        makeDisplayLists();
        break;

    case Action_ResetView:
        m_camera.SetRotation(eye);
        m_camera.SetCenter(Vec3f(0,0,0));
        break;

    case Action_WriteOBJ:
        for (unsigned i=0; i<n_surfaceNames.size(); i++)
        {
            string filename = "surface_" + n_surfaceNames[i] + string(".obj");
            ofstream out(filename.c_str());
            if (!out)
            {
                m_commonCtrl.message(sprintf("Could not open file %s, skipping", filename.c_str()));
                out.close();
                continue;
            }
            else
            {
                outputObjFile(out, m_surfaces[i]);
                m_commonCtrl.message(sprintf("Wrote %s", filename.c_str()));
            }
        }
        break;

    default:
        FW_ASSERT(false);
        break;
    }

    m_window.setVisible(true);
    if (ev.type == Window::EventType_Paint)
    {
        drawScene();
    }
    m_window.repaint();
    return false;
}

//------------------------------------------------------------------------

void App::keyDownFunc(const Window::Event& ev)
{
    int x = ev.mousePos[0];
    int y = ev.mousePos[1];

    if (ev.key == FW_KEY_MOUSE_LEFT)
    {
        m_mousePressed = true;
        m_camera.MouseClick(Camera::LEFT, x, y);
    }
    else if (ev.key == FW_KEY_MOUSE_MIDDLE)
    {
        m_mousePressed = true;
        m_camera.MouseClick(Camera::MIDDLE, x, y);
    }
    else if (ev.key == FW_KEY_MOUSE_RIGHT)
    {
        m_mousePressed = true;
        m_camera.MouseClick(Camera::RIGHT, x, y);
    }
}

//------------------------------------------------------------------------

void App::keyUpFunc(const Window::Event& ev)
{
    int x = ev.mousePos[0];
    int y = ev.mousePos[1];
    if (ev.key == FW_KEY_MOUSE_LEFT || ev.key == FW_KEY_MOUSE_MIDDLE || ev.key == FW_KEY_MOUSE_RIGHT)
    {
        m_camera.MouseRelease(x,y);
        m_mousePressed = false;
    }
}

//------------------------------------------------------------------------

// Called when mouse is moved.
void App::motionFunc(const Window::Event& ev)
{
    if (m_mousePressed)
    {
        int x = ev.mousePos[0];
        int y = ev.mousePos[1];
        m_camera.MouseDrag(x,y);        
    }
}

//------------------------------------------------------------------------

// Called when the window is resized
// w, h - width and height of the window in pixels.
void App::reshapeFunc(int w, int h)
{
    m_camera.SetDimensions(w,h);

    m_camera.SetViewport(0,0,w,h);
    m_camera.ApplyViewport();

    // Set up a perspective view, with square aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    m_camera.SetPerspective(50);
    m_camera.ApplyPerspective();
}

//------------------------------------------------------------------------

// This function is responsible for displaying the object.
void App::drawScene(void)
{
    // Remove any shader that may be in use.
    glUseProgram(0);

    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Vec2i size = m_window.getSize();
    reshapeFunc(size[0], size[1]);

    glMatrixMode( GL_MODELVIEW );  
    glLoadIdentity();              

    // Light color (RGBA)
    GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
    GLfloat Lt0pos[] = {3.0,3.0,5.0,1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

    m_camera.ApplyModelview();

    // Call the relevant display lists.
    if (m_surfaceMode)
        glCallList(m_surfaceLists[m_surfaceMode]);

    if (m_curveMode)
        glCallList(m_curveLists[m_curveMode]);

    // This draws the coordinate axes when you're rotating, to
    // keep yourself oriented.
    if (m_mousePressed)
    {
        glPushMatrix();
        glTranslated(m_camera.GetCenter()[0], m_camera.GetCenter()[1], m_camera.GetCenter()[2]);
        glCallList(m_axisList);
        glPopMatrix();
    }

    if (m_pointMode)
        glCallList(m_pointList);
}

//------------------------------------------------------------------------

// Initialize OpenGL's rendering modes
void App::initRendering()
{
    glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);       // Turn on light #0.

    // Setup polygon drawing
    glShadeModel(GL_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Antialiasing
    // This looks like crap
    /*
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);    
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    */

    // Clear to black
    glClearColor(0,0,0,1);

    // Base material colors (they don't change)
    GLfloat diffColor[] = {0.4, 0.4, 0.4, 1};
    GLfloat specColor[] = {0.9, 0.9, 0.9, 1};
    GLfloat shininess[] = {50.0};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    m_curveLists[1] = glGenLists(1);
    m_curveLists[2] = glGenLists(1);
    m_surfaceLists[1] = glGenLists(1);
    m_surfaceLists[2] = glGenLists(1);
    m_axisList = glGenLists(1);
    m_pointList = glGenLists(1);
}

//------------------------------------------------------------------------

// Load in objects from standard input into the class member variables: 
// m_ctrlPoints, m_curves, m_curveNames, m_surfaces, m_surfaceNames.  If
// loading fails, this will exit the program.
void App::loadObjects(const string& fileName)
{
    ifstream in(fileName.c_str());

    cout << endl << "*** loading and constructing curves and surfaces ***" << endl;

    if (!parseFile(in, m_ctrlPoints, m_curves, m_curveNames, m_surfaces, n_surfaceNames))
    {
        cerr << "\aerror in file format\a" << endl;
        in.close();
        exit(-1);              
    }

    in.close();

    cerr << endl << "*** done ***" << endl;
}

//------------------------------------------------------------------------

void App::writeObjects(const string& fileName)
{
    cerr << endl << "*** writing obj files ***" << endl;

    string prefix (fileName.c_str());

    for (unsigned i=0; i<n_surfaceNames.size(); i++)
    {
        if (n_surfaceNames[i] != ".")
        {
            string filename =
                prefix + string("_")
                + n_surfaceNames[i]
            + string(".obj");

            ofstream out(filename.c_str());

            if (!out)
            {
                cerr << "\acould not open file " << filename << ", skipping"<< endl;
                out.close();
                continue;
            }
            else
            {
                outputObjFile(out, m_surfaces[i]);
                cerr << "wrote " << filename.c_str() <<  endl;
            }
        }
    }
}

//------------------------------------------------------------------------

void App::makeDisplayLists()
{
    

    // Compile the display lists

    glNewList(m_curveLists[1], GL_COMPILE);
    {
        for (unsigned i=0; i<m_curves.size(); i++)
            drawCurve(m_curves[i], 0.0);
    }
    glEndList();

    glNewList(m_curveLists[2], GL_COMPILE);
    {
        for (unsigned i=0; i<m_curves.size(); i++)
            drawCurve(m_curves[i], m_lineLen);
    }
    glEndList();

    glNewList(m_surfaceLists[1], GL_COMPILE);
    {
        for (unsigned i=0; i<m_surfaces.size(); i++)
            drawSurface(m_surfaces[i], true);
    }
    glEndList();

    glNewList(m_surfaceLists[2], GL_COMPILE);
    {
        for (unsigned i=0; i<m_surfaces.size(); i++)
        {
            drawSurface(m_surfaces[i], false);
            drawNormals(m_surfaces[i], m_lineLen);
        }
    }
    glEndList();

    glNewList(m_axisList, GL_COMPILE);
    {
        // Save current state of OpenGL
        glPushAttrib(GL_ALL_ATTRIB_BITS);

        // This is to draw the axes when the mouse button is down
        glDisable(GL_LIGHTING);
        glLineWidth(3);
        glPushMatrix();
        glScaled(5.0,5.0,5.0);
        glBegin(GL_LINES);
        glColor4f(1,0.5,0.5,1); glVertex3d(0,0,0); glVertex3d(1,0,0);
        glColor4f(0.5,1,0.5,1); glVertex3d(0,0,0); glVertex3d(0,1,0);
        glColor4f(0.5,0.5,1,1); glVertex3d(0,0,0); glVertex3d(0,0,1);

        glColor4f(0.5,0.5,0.5,1);
        glVertex3d(0,0,0); glVertex3d(-1,0,0);
        glVertex3d(0,0,0); glVertex3d(0,-1,0);
        glVertex3d(0,0,0); glVertex3d(0,0,-1);

        glEnd();
        glPopMatrix();

        glPopAttrib();
    }
    glEndList();

    glNewList(m_pointList, GL_COMPILE);
    {
        // Save current state of OpenGL
        glPushAttrib(GL_ALL_ATTRIB_BITS);

        // Setup for point drawing
        glDisable(GL_LIGHTING);    
        glColor4f(1,1,0.0,1);
        glPointSize(4);
        glLineWidth(1);

        for (unsigned i=0; i<m_ctrlPoints.size(); i++)
        {
            glBegin(GL_POINTS);
            for (unsigned j=0; j<m_ctrlPoints[i].size(); j++)
                glVertex(m_ctrlPoints[i][j]);
            glEnd();

            glBegin(GL_LINE_STRIP);
            for (unsigned j=0; j<m_ctrlPoints[i].size(); j++)
                glVertex(m_ctrlPoints[i][j]);
            glEnd();
        }

        glPopAttrib();
    }
    glEndList();

}

void App::screenshot (const String& name)
{
    // Capture image.

    const Vec2i& size = m_window.getGL()->getViewSize();
    Image image(size, ImageFormat::R8_G8_B8_A8);
    glUseProgram(0);
    glWindowPos2i(0, 0);
    glReadPixels(0, 0, size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, image.getMutablePtr());

    // Display the captured image immediately.

    for (int i = 0; i < 3; i++)
    {
        glDrawPixels(size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, image.getPtr());
        m_window.getGL()->swapBuffers();
    }
    glDrawPixels(size.x, size.y, GL_RGBA, GL_UNSIGNED_BYTE, image.getPtr());

    // Export.

    image.flipY();
    exportImage(name, &image);
    printf("Saved screenshot to '%s'", name.getPtr());
}

//------------------------------------------------------------------------

void FW::init(void)
{
    new App;
}

//------------------------------------------------------------------------
