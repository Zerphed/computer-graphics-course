#define _USE_MATH_DEFINES

#include "App.hpp"
#include "base/Main.hpp"
#include "gpu/GLContext.hpp"
#include "io/File.hpp"
#include "io/StateDump.hpp"
#include "base/Random.hpp"
#include "extra.h"

#include <stdio.h>
#include <conio.h>

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <GL/GLU.h>

using namespace FW;
using namespace std;

const unsigned int NUM_JOINTS = 18;
static const char jointNames[NUM_JOINTS][256] = { "Root", "Chest", "Waist", "Neck", "Right hip", "Right leg", "Right knee", "Right foot", "Left hip", "Left leg", "Left knee", "Left foot", "Right collarbone", "Right shoulder", "Right elbow", "Left collarbone", "Left shoulder", "Left elbow" };

//------------------------------------------------------------------------

App::App(void)
:   m_action        (Action_None),
    m_commonCtrl    (CommonControls::Feature_Default & ~CommonControls::Feature_RepaintOnF5),
    m_cullMode      (CullMode_None),

    m_mousePressed  (false),
    m_drawSkeleton  (true),
    m_drawAxes      (true),


    m_x             (0.0f),
    m_y             (0.0f),
    m_z             (0.0f),
    m_sliderChanged (false),
    m_jointChanged  (false),
    
    m_controlIndex  (0)
    
   
{
    m_window.setTitle("basis");
    m_window.addListener(this);
    m_window.addListener(&m_commonCtrl);

    String name = m_window.showFileLoadDialog("Load model");
    if (name.getLength())
        loadModel(name);

    // Make ui controls.
    for (unsigned i = 0; i < NUM_JOINTS; ++i)
    {
       m_controls.push_back(tuple<ModelerControl, 3>(  ModelerControl(&jointNames[i][0], -M_PI, M_PI, 0.1f, 0),
                                                       ModelerControl(&jointNames[i][0], -M_PI, M_PI, 0.1f, 0),
                                                       ModelerControl(&jointNames[i][0], -M_PI, M_PI, 0.1f, 0)));
    }

    updateUI();

    m_window.getGL()->swapBuffers();

    initRendering();

    m_camera.SetDistance( 2 );
    m_camera.SetCenter( Vec3f( 0.5, 0.5, 0.5 ) );
    
}

//------------------------------------------------------------------------

App::~App(void)
{}

//------------------------------------------------------------------------

bool App::handleEvent(const Window::Event& ev)
{

    if (m_jointChanged)
    {
        m_jointChanged = false;
        updateUI();
    }

    if (m_sliderChanged)
    {
        m_sliderChanged = false;
        updateModel();
    }


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

    Action action   = m_action;
    m_action        = Action_None;
    String name;

    switch (action)
    {
    case Action_None:
        break;

    case Action_ResetView:
        m_camera.SetRotation(Mat4f());
        m_camera.SetDistance( 2 );
        m_camera.SetCenter( Vec3f( 0.5, 0.5, 0.5 ) );
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
    glUseProgram(0);
    
    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Vec2i size = m_window.getSize();
    reshapeFunc(size[0], size[1]);

    glMatrixMode( GL_MODELVIEW );  
    glLoadIdentity();              


    // Note that the lighting is applied *before* applying the camera
    // transform.  This is so the light appeared fixed on the camera.    
    GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
    GLfloat Lt0pos[] = {3.0,3.0,5.0,1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

    // These are just some default material colors
    GLfloat diffColor[] = {0.4f, 0.4f, 0.4f, 1.f};
    GLfloat specColor[] = {0.6f, 0.6f, 0.6f, 1.f};
    GLfloat shininess[] = {50.0f};
    
    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColor );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specColor );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, shininess );

    //m_camera.ApplyModelview();
    glLoadMatrixf(&m_camera.viewMatrix()(0, 0));

    if (m_drawAxes)
    {
        glPushMatrix();
        glTranslated(m_camera.GetCenter()[0], m_camera.GetCenter()[1], m_camera.GetCenter()[2]);
        drawAxes();
        glPopMatrix();
    }

    m_model.draw( m_camera.viewMatrix(), m_drawSkeleton );
    
}

//------------------------------------------------------------------------

// Initialize OpenGL's rendering modes
void App::initRendering()
{
    // Setup opengl
    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glEnable( GL_NORMALIZE );
    glClearColor(0,0,0,1);
}

//------------------------------------------------------------------------

// Load in objects from standard input into the global variables: 
// gCtrlPoints, gCurves, gCurveNames, gSurfaces, gSurfaceNames.  If
// loading fails, this will exit the program.
void App::loadModel(const String& fileName)
{
    int end = fileName.lastIndexOf('.');
    String prefix = (end > 0) ? fileName.substring(0, end) : fileName;
    String skeletonFile = prefix + ".skel";
    String meshFile = prefix + ".obj";
    String attachmentsFile = prefix + ".attach";

    cout << "skeleton:   " << skeletonFile.getPtr() << endl;
    cout << "mesh:       " << meshFile.getPtr() << endl;
    cout << "attachment: " << attachmentsFile.getPtr() << endl;

    m_model.load(skeletonFile.getPtr(), meshFile.getPtr(), attachmentsFile.getPtr());
}

//------------------------------------------------------------------------

void App::writeObjects(const string& fileName)
{
    cerr << fileName << endl;
}

//------------------------------------------------------------------------

void App::updateModel    (void)
{
    // update the skeleton from sliders
	updateJoints();

    // Update the bone to world transforms for SSD.
	m_model.updateCurrentJointToWorldTransforms();

	// update the mesh given the new skeleton
	m_model.updateMesh();
}

//------------------------------------------------------------------------

void App::updateJoints(void)
{
    for (unsigned i = 0; i < m_controls.size(); ++i)
    {
        float x = m_controls[i][0].m_value;
        float y = m_controls[i][1].m_value;
        float z = m_controls[i][2].m_value;
        
        m_model.setJointTransform(i, x, y, z);
    }
}

//------------------------------------------------------------------------

void App::updateUI  (void)
{
    m_commonCtrl.resetControls();

    m_commonCtrl.showFPS(true);
    m_commonCtrl.addButton(&m_drawSkeleton, FW_KEY_S, "Draw skeleton... (S)", NULL);
    m_commonCtrl.addButton(&m_drawAxes, FW_KEY_X,     "Draw axes... (X)", NULL);
    m_commonCtrl.addButton((S32*)&m_action, Action_ResetView, FW_KEY_SPACE, "Reset view... (SPACE)");
    m_commonCtrl.addSeparator();

    // Make ui controls.
    for (unsigned i = 0; i < NUM_JOINTS; ++i)
    {
        m_commonCtrl.addToggle((S32*)&m_controlIndex, i, FW_KEY_NONE, m_controls[i][0].m_name, &m_jointChanged);
    }

    ModelerControl& ctrl_x = m_controls[m_controlIndex][0];
    ModelerControl& ctrl_y = m_controls[m_controlIndex][1];
    ModelerControl& ctrl_z = m_controls[m_controlIndex][2];
    m_commonCtrl.addSlider ((F32*)&ctrl_x.m_value, ctrl_x.m_minimum, ctrl_x.m_maximum, false, FW_KEY_NONE, FW_KEY_NONE, "x", 0.0f, &m_sliderChanged);
    m_commonCtrl.addSlider ((F32*)&ctrl_y.m_value, ctrl_y.m_minimum, ctrl_y.m_maximum, false, FW_KEY_NONE, FW_KEY_NONE, "y", 0.0f, &m_sliderChanged);
    m_commonCtrl.addSlider ((F32*)&ctrl_z.m_value, ctrl_z.m_minimum, ctrl_z.m_maximum, false, FW_KEY_NONE, FW_KEY_NONE, "z", 0.0f, &m_sliderChanged);
}

void App::drawAxes(void)
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

//------------------------------------------------------------------------

void FW::init(void)
{
    new App;
}

//------------------------------------------------------------------------
