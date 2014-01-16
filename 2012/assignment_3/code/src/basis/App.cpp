#define _USE_MATH_DEFINES

#include "App.hpp"
#include "base/Main.hpp"
#include "gpu/GLContext.hpp"
#include "io/File.hpp"
#include "io/StateDump.hpp"
#include "base/Random.hpp"
#include "extra.h"

#include "clothSystem.h"
#include "pendulumSystem.h"
#include "simpleSystem.h"
#include "integratorEuler.h"
#include "integratorTrapezoidal.h"
#include "integratorRungeKutta.h"

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

//------------------------------------------------------------------------

App::App(void)
:   m_action                (Action_None),
    m_commonCtrl            (CommonControls::Feature_Default & ~CommonControls::Feature_RepaintOnF5),
    m_cullMode              (CullMode_None),
    m_polygonMode           (PolygonMode_WireFrame),
    m_integratorMode        (IntegratorMode_Euler),
    m_particleSystemType    (ParticleSystemType_SimpleSystem),

    m_mousePressed          (false),
    m_drawAxes              (true),

	m_wind                  (false),

    m_particleSystemChanged (true),
    m_integratorChanged     (true),

    m_particleSystem        (0),
    m_integrator            (0),
    m_interval              (5),
    m_stepSize              (0.01)
   
{
    m_window.setTitle("Particle System Simulation");
    m_window.addListener(this);
    m_window.addListener(&m_commonCtrl);

    m_commonCtrl.showFPS(true);

    m_commonCtrl.addSlider ((F32*)&m_stepSize, 0.0001, 0.04, false, FW_KEY_NONE, FW_KEY_NONE, "Step size", 0.0f, &m_integratorChanged);

    m_commonCtrl.addButton((S32*)&m_action,             Action_ResetView,               FW_KEY_SPACE,   "Reset view... (Space)");
    m_commonCtrl.addSeparator();

    m_commonCtrl.addButton((S32*)&m_action,             Action_Shift_Left,              FW_KEY_LEFT,    "Shift system left... (Left)");
    m_commonCtrl.addButton((S32*)&m_action,             Action_Shift_Right,             FW_KEY_RIGHT,   "Shift system right... (Right)");
    m_commonCtrl.addSeparator();

    m_commonCtrl.addToggle((S32*)&m_polygonMode,        PolygonMode_WireFrame,          FW_KEY_NONE,    "Draw wireframe");
    m_commonCtrl.addToggle((S32*)&m_polygonMode,        PolygonMode_Smooth,             FW_KEY_NONE,    "Smooth shading.");
    m_commonCtrl.addSeparator();

	m_commonCtrl.addButton((S32*)&m_action,             Action_Emulate_Wind,            FW_KEY_W,       "Emulate wind (W)");
	m_commonCtrl.addSeparator();

    m_commonCtrl.addToggle((S32*)&m_integratorMode,     IntegratorMode_Euler,           FW_KEY_E,       "Euler... (E)",         &m_integratorChanged);
    m_commonCtrl.addToggle((S32*)&m_integratorMode,     IntegratorMode_Trapezoidal,     FW_KEY_T,       "Trapetzoid... (T)",    &m_integratorChanged);
	m_commonCtrl.addToggle((S32*)&m_integratorMode,     IntegratorMode_RungeKutta,      FW_KEY_R,       "Runge Kutta... (R)",   &m_integratorChanged);
    m_commonCtrl.addSeparator();

    m_commonCtrl.addToggle((S32*)&m_particleSystemType, ParticleSystemType_Cloth,        FW_KEY_C,       "Cloth... (C)",         &m_particleSystemChanged);
    m_commonCtrl.addToggle((S32*)&m_particleSystemType, ParticleSystemType_Pendulum,     FW_KEY_P,       "Pendulum... (P)",      &m_particleSystemChanged);
    m_commonCtrl.addToggle((S32*)&m_particleSystemType, ParticleSystemType_SimpleSystem, FW_KEY_S,       "Simple system... (S)", &m_particleSystemChanged);

    m_window.getGL()->swapBuffers();

    initRendering();


    m_camera.SetRotation(Mat4f());
    m_camera.SetDistance( 6 );
    m_camera.SetCenter( Vec3f( 0.5, 0.5, 0.5 ) );

}

//------------------------------------------------------------------------

App::~App(void)
{
    if (m_integrator)
        delete m_integrator;
}

//------------------------------------------------------------------------

bool App::handleEvent(const Window::Event& ev)
{

    if (ev.type == Window::EventType_Close)
    {
        m_window.showModalMessage("Exiting...");
        delete this;
        return true;
    }

    if (m_particleSystemChanged || m_integratorChanged)
    {
        m_particleSystemChanged = false;
        m_integratorChanged = false;
        resetSystem(m_particleSystemType, m_integratorMode, 10, 10);
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
        m_camera.SetDistance( 6 );
        m_camera.SetCenter( Vec3f( 0.5, 0.5, 0.5 ) );
        break;

    case Action_Shift_Left:
        if (m_particleSystem)
            m_particleSystem->shiftSystem(true);
        break;

    case Action_Shift_Right:
        if (m_particleSystem)
            m_particleSystem->shiftSystem(false);
        break;

	case Action_Emulate_Wind:
		if (m_particleSystem && m_particleSystemType == ParticleSystemType_Cloth)
			m_particleSystem->toggleWind();
		break;

    default:
        FW_ASSERT(false);
        break;
    }

    // Check m_timer. If enough time has passed, update particle system.
    float secs = m_timer.getElapsed();
    int msecs = secs * 1000;
    if (msecs > m_interval)
    {
        stepSystem();
        m_timer.start();
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

    //m_camera.ApplyModelview();
    glLoadMatrixf(&m_camera.viewMatrix()(0, 0));

    GLfloat particleColor[] = {0.4f, 0.7f, 1.0f, 1.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, particleColor);

    if (m_polygonMode == PolygonMode_Smooth)
    {
        m_particleSystem->draw();
    }
    else if (m_polygonMode == PolygonMode_WireFrame)
    {
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK,  GL_LINE);
        m_particleSystem->draw();
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK,  GL_FILL);
    }

    if (m_mousePressed)
    {
        glPushMatrix();
        glTranslated(m_camera.GetCenter()[0], m_camera.GetCenter()[1], m_camera.GetCenter()[2]);
        drawAxes();
        glPopMatrix();
    }

    //m_model.draw( m_camera.viewMatrix(), m_drawSkeleton );
    
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

void App::writeObjects(const string& fileName)
{
    cerr << fileName << endl;
}

//------------------------------------------------------------------------

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

void App::resetSystem    (ParticleSystemType particleSystemType, IntegratorMode integratorMode, int rows, int cols)
{
    // Integrator owns the particle system, that is, the destructor of Integrator
    // deletes the particle system.
    if (m_integrator)
        delete m_integrator;

    if (particleSystemType == ParticleSystemType_Cloth)
    {
        m_particleSystem = new ClothSystem(rows, cols);
    }
    else if (particleSystemType == ParticleSystemType_Pendulum) 
    {
        m_particleSystem = new PendulumSystem(rows);
    }
    else if (particleSystemType == ParticleSystemType_SimpleSystem)
    {
        m_particleSystem = new SimpleSystem;
    }
    else
    {
        cout << "Using default particle system: SimpleSystem.\n";
        m_particleSystem = new SimpleSystem;
    }

    if (integratorMode == IntegratorMode_Euler)
    {
        m_integrator = new EulerIntegrator(m_stepSize, m_particleSystem);
    }
    else if (integratorMode == IntegratorMode_Trapezoidal)
    {
        m_integrator = new TrapezoidalIntegrator(m_stepSize, m_particleSystem);
    }
	else if (integratorMode == IntegratorMode_RungeKutta) {
		m_integrator = new RungeKuttaIntegrator(m_stepSize, m_particleSystem);
	}
    else
    {
        cout << "Using default integrator: Euler.\n";
        m_integrator = new EulerIntegrator(m_stepSize, m_particleSystem);
    }

    // Restart timer. Next time we call m_timer.getElapsed() we get the time in seconds
    // that has passed since this point. Calling m_timer.start() again will restart the timer.
    m_timer.start();
}

//------------------------------------------------------------------------

void App::stepSystem     (void)
{
    m_integrator->stepSystem();
}

//------------------------------------------------------------------------

void App::drawSystem     (void)
{
    GLfloat particleColor[] = {0.4f, 0.7f, 1.0f, 1.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, particleColor);

    m_particleSystem->draw();
}

//------------------------------------------------------------------------

void FW::init(void)
{
    new App;
}

//------------------------------------------------------------------------
