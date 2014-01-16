#include "App.hpp"
#include "base/Main.hpp"
#include "base/Timer.hpp"
#include "gpu/GLContext.hpp"

#include <stdio.h>
#include <conio.h>

#include <iostream>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <GL/GLU.h>
#include "3rdparty/glut.h"

using namespace FW;
using namespace std;

// Utilities
//------------------------------------------------------------------------
#define PI 3.14159265359

// Returns an arcball vector created according to the parameters (mouse)
// coordinates.
Vec3f App::getArcballVec(GLfloat x, GLfloat y) {
	Vec2i windowSize = m_window.getSize();

	// Initialize the OP vector
	Vec3f OPVector;
	OPVector[0] = x/(GLfloat)windowSize.x*2.0f - 1.0f;
	OPVector[1] = y/(GLfloat)windowSize.y*2.0f - 1.0f;
	OPVector[2] = 0.0f;

	OPVector[1] *= -1.0f;
	
	GLfloat OPSquared = (OPVector[0]*OPVector[0]) + (OPVector[1]*OPVector[1]);
	if (OPSquared <= 1.0f)
		OPVector[2] = FW::sqrt(1.0f-OPSquared);
	else
		OPVector = FW::normalize(OPVector);

	return OPVector;
}

// Returns the angle between the two vectors in radians
GLfloat App::getRotationAngle(const Vec3f& va, const Vec3f& vb) {
	GLfloat angle = (GLfloat)acos(min(1.0f, va.dot(vb)));
	return angle;
}

// Rotates the model
void App::rotateModel() {
	Vec3f va = getArcballVec((GLfloat)mousePrevCoord.x, (GLfloat)mousePrevCoord.y);
	Vec3f vb = getArcballVec((GLfloat)mouseCurCoord.x, (GLfloat)mouseCurCoord.y);

	// In radians here
	GLfloat angle = getRotationAngle(va, vb);
	
	// Threshold check, smaller values might cause problems
	if (angle < 0.0009f) 
		return;

	// Convert to degrees
	angle *= (180.0f/PI);

	// Get the axis in the view coodinates
	Vec3f axisInViewCoordinates = va.cross(vb);
	axisInViewCoordinates.normalize();

	// Debug, shows the angle and axis information
	//cout << "A: " << angle << endl;
	//cout << "X: " << axisInViewCoordinates[0] << " Y: " << axisInViewCoordinates[1] << " Z: " << axisInViewCoordinates[2] << endl;
}

void App::rotateCamera() {
	Vec3f va = getArcballVec((GLfloat)mousePrevCoord.x, (GLfloat)mousePrevCoord.y);
	Vec3f vb = getArcballVec((GLfloat)mouseCurCoord.x, (GLfloat)mouseCurCoord.y);

	// In radians here
	GLfloat angle = getRotationAngle(va, vb);
	
	if (angle < 0.0009f) 
		return;

	// Convert to degrees
	angle *= (180.0f/PI);
}

//------------------------------------------------------------------------
const static GLfloat diffColors[4][4] = { {0.5, 0.5, 0.9, 1.0},
										  {0.9, 0.5, 0.5, 1.0},
										  {0.5, 0.9, 0.3, 1.0},
										  {0.3, 0.8, 0.9, 1.0} };

//------------------------------------------------------------------------
App::App(void)
:   m_action        (Action_None),
    m_commonCtrl    (CommonControls::Feature_Default & ~CommonControls::Feature_RepaintOnF5),
    m_cullMode      (CullMode_None),
	colorIndex		(0),
	prevColorIndex  (0),
	changingColor	(false),
	mouseLeftPressed (false),
	mouseRightPressed (false),
	rotating (false),
	rotatedAngle(0),
	rotationTimer(new Timer()),
	colorTimer(new Timer())
{
    m_window.setTitle("Mesh loader");
    m_window.addListener(this);
    m_window.addListener(&m_commonCtrl);

    m_window.getGL()->swapBuffers();

	// Initialize current color to the first
	currentColor[0] = currentColor[1] = 0.5f;
	currentColor[2] = 0.9f;
	currentColor[3] = 1.0f;

	// Initialize the camera position
	// Position the camera at [0,0,5]
	cameraPos[0] = cameraPos[1] = 0.0f;
	cameraPos[2] = 5.0f;

	// Initialise the light position
	Lt0pos[0] = Lt0pos[1] = Lt0pos[3] = 1.0f;
	Lt0pos[2] = 5.0f;

	// Call the load mesh to prompt for a mesh
	String fileName = m_window.showFileLoadDialog("Load mesh");
    loadMesh(fileName);

    initRendering();
}

//------------------------------------------------------------------------

App::~App(void) {
	// Delete the display list (remember your manners - clean up your own mess)
	glDeleteLists(DLid, 1);
	delete rotationTimer;
	delete colorTimer;
}

//------------------------------------------------------------------------

bool App::handleEvent(const Window::Event& ev)
{
	if (changingColor) {
		// Total time in use for the color change and time step for one delta value to be
		// added.
		GLfloat time=4.0f, timeStep=0.1f;

		if (colorTimer->getTotal() >= time) {
			changingColor = false;

			// Current color set to absolutely correct value, floats cause errors
			for (GLint i = 0; i < 4; i++)
				currentColor[i] = diffColors[colorIndex][i]; 

			// Reset and stop the whole timer
			colorTimer->unstart();
			colorTimer->clearTotal();
		}
		else if (colorTimer->getElapsed() >= timeStep) {
			// Delta for each color component
			GLfloat dr = (diffColors[colorIndex][0] - diffColors[prevColorIndex][0]) / (time/timeStep),
					dg = (diffColors[colorIndex][1] - diffColors[prevColorIndex][1]) / (time/timeStep), 
					db = (diffColors[colorIndex][2] - diffColors[prevColorIndex][2]) / (time/timeStep);

			currentColor[0] += dr;
			currentColor[1] += dg;
			currentColor[2] += db;

			// Reset the timer interval
			colorTimer->end();
		}
	}


	if (rotating) {
		GLfloat timeStep = 0.01f;

		if (rotationTimer->getElapsed() >= timeStep) {
			rotatedAngle = (rotatedAngle+1)%360;

			// Reset the timer interval
			rotationTimer->end();
		}
	}

    if (ev.type == Window::EventType_Close)
    {
        m_window.showModalMessage("Exiting...");
        delete this;
        return true;
    }
	else if (ev.type == Window::EventType_Mouse)
	{
		// If the window registers that the mouse is dragging we either rotate the camera or the object
		if (ev.mouseDragging) {
			// Update the current mouse coordinates
			mouseCurCoord = m_window.getMousePos();

			// Decide whether to rotate the model or the camera
			if (mouseLeftPressed) {
				// TODO: Rotate the model - unfinished
				rotateModel();
			}
			if (mouseRightPressed) {
				// TODO: Rotate the camera - unfinished
				rotateCamera();
			}

			// Update the previous mouse coordinates
			mousePrevCoord = mouseCurCoord;
		}
	}
	else if (ev.type == Window::EventType_KeyDown)
    {
        // If the user has pressed a key we call our own function handleKeyDown.
        handleKeyDown(ev.key);
    }
	else if (ev.type == Window::EventType_KeyUp)
	{
		// If the user releases a key we call our own function handleKeyUp.
		handleKeyUp(ev.key);
	}

    Action action   = m_action;
    m_action        = Action_None;
    String name;

    switch (action)
    {
    case Action_None:
        break;

    default:
        FW_ASSERT(false);
        break;
    }

    m_window.setVisible(true);
    if (ev.type == Window::EventType_Paint)
        renderFrame();
    m_window.repaint();

    return false;
}

//------------------------------------------------------------------------

GLuint App::createDL(void) {
	// Get an identifier for the display list
	GLuint id = glGenLists(1);

	size_t faces = m_vecf.size();

	// Generate the display list
	glNewList(id, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	for (size_t i = 0; i < faces; ++i) {
		try {
			glNormal3d(m_vecn[m_vecf[i][1]-1][0], m_vecn[m_vecf[i][1]-1][1], m_vecn[m_vecf[i][1]-1][2]);
			glVertex3d(m_vecv[m_vecf[i][0]-1][0], m_vecv[m_vecf[i][0]-1][1], m_vecv[m_vecf[i][0]-1][2]);

			glNormal3d(m_vecn[m_vecf[i][3]-1][0], m_vecn[m_vecf[i][3]-1][1], m_vecn[m_vecf[i][3]-1][2]);
			glVertex3d(m_vecv[m_vecf[i][2]-1][0], m_vecv[m_vecf[i][2]-1][1], m_vecv[m_vecf[i][2]-1][2]);

			glNormal3d(m_vecn[m_vecf[i][5]-1][0], m_vecn[m_vecf[i][5]-1][1], m_vecn[m_vecf[i][5]-1][2]);
			glVertex3d(m_vecv[m_vecf[i][4]-1][0], m_vecv[m_vecf[i][4]-1][1], m_vecv[m_vecf[i][4]-1][2]);
		}
		catch (const std::exception& e) {
			cerr << e.what() << endl;
			cerr << "Error: index out of bounds during mesh initialization, in createDL" << endl;
		}
	}
	glEnd();
	glEndList();

	return id;
}

void App::renderFrame(void)
{
    // Always use the largest square viewport possible
    // Request size of window
    Vec2i size = m_window.getSize();
    int w = size[0];
    int h = size[1];
    // glViewport(x, y, w, h) tells opengl to render on a rectangle of width w
    // and height h. The upper corner of the rectangle is at (x, y). The coordinates
    // are relative to the application window.
    if (w > h) 
        glViewport((w - h) / 2, 0, h, h);
    else
        glViewport(0, (h - w) / 2, w, w);
    

    //// Set up a perspective view, with square aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 50 degree fov, uniform aspect ratio, near = 1, far = 100
    gluPerspective(50.0, 1.0, 1.0, 100.0);

    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rotate the image
    glMatrixMode( GL_MODELVIEW );  // Current matrix affects objects positions
    glLoadIdentity();              // Initialize to the identity

    // Position the camera at [0,0,5], looking at [0,0,0],
    // with [0,1,0] as the up direction.
	gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2],
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // Set material properties of object

    // Here we use the first color entry as the diffuse color
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, currentColor);

    // Define specular color and shininess
    GLfloat specColor[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shininess[] = {100.0};

    // Note that the specular color and shininess can stay constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  
    // Set light properties

    // Light color (RGBA)
    GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
    
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);
	
	glPushMatrix();
	glRotatef(rotatedAngle, 0.0f, 1.0f, 0.0f);

    // Draw the loaded object
	glCallList(DLid);

	glPopMatrix();
}
//------------------------------------------------------------------------

void App::loadMesh (const String& fileName)
{
	// Load the new mesh and create new display lists

    char buffer[1024];

    m_vecv.clear();
    m_vecn.clear();
    m_vecf.clear();

    // Open input file stream for reading.
    ifstream in(fileName.getPtr(), ios::in);

    // Read a line into the buffer. The newline character '\n' is 
    // discarded by getline function.
    while (in.getline(buffer, 1024))
    {

        // Replace '/' characters with spaces ' '. Once the values are separated
        // by spaces, we can read them with input streams >> operator.
        for (char *c = &buffer[0]; *c != '\0'; c++)
        {
            if (*c == '/')
                *c = ' ';
        }

        // Face index array
        vector<unsigned>    u(6);
        Vec3f               v;
        string              s;

        // stringstream can be operated like any other stream
        stringstream        ss(buffer);
        // Read the first token from the line to s, the first token
        // identifies the type of object (vertex or normal or ...)
        ss >> s;

        if (s == "v")
        {
            // Read the three vertex coordinates (x, y, z) into v.
            // store the Vec3f in the class member variable m_vecv.
			ss >> v[0] >> v[1] >> v[2];
			m_vecv.push_back(v);
        }
        else if (s == "vn")
        {
            ss >> v[0] >> v[1] >> v[2];
			m_vecn.push_back(v);
        }
        else if (s == "f")
        {
            // The faces are stored in 
            // f v1/vt1/vn1 v2/vt2/vn2 ... (Note that we replaced the '/' characters with whitespaces)
            // format where
            //  
            //  vi = vertex index
            //  vti = texture index
            //  vni = normal index
            //
            // Since we are not using textures in this exercise
            // you can read them into a temporary variable.

            // A temporary variable for reading the unusued texture indices.
            unsigned tmp;
			ss >> u[0] >> tmp >> u[1] >> u[2] >> tmp >> u[3] >> u[4] >> tmp >> u[5];
			m_vecf.push_back(u);

            // Debug: It might be a good idea to print the indices to see that they were read correctly
            // cerr << u[0] << " " <<u[1] << " " <<u[2] << " " <<u[3] << " " <<u[4] << " " <<u[5] << endl;
        }
    }

	// I fixed your memory leak for you, remember closing the file stream.
	in.close();

	// Create a display list for the loaded mesh
	DLid = createDL();
}

//------------------------------------------------------------------------

void App::handleKeyDown(const String& key)
{

    // Right click -> Go to definition 
    // on FW_KEY_C to see definitions for other keys.
    if (key == FW_KEY_C)        
    { 
		if (!changingColor) {
			changingColor = true;
			colorTimer->start();
			prevColorIndex = colorIndex;
			colorIndex = (colorIndex+1)%(sizeof(diffColors)/sizeof(*diffColors));
		}
    }
    else if (key == FW_KEY_UP)       
    { 
        Lt0pos[1] += 0.5f;
    }
    else if (key == FW_KEY_DOWN)     
    {
		Lt0pos[1] -= 0.5f;
    }
    else if (key == FW_KEY_LEFT)     
    {
        Lt0pos[0] -= 0.5f;
    }
    else if (key == FW_KEY_RIGHT)    
    {
        Lt0pos[0] += 0.5f;
    }
    else if (key == FW_KEY_L)
    {
		// Get the handle for the old display list
		GLuint oldId = DLid;

        String fileName = m_window.showFileLoadDialog("Load mesh");
        loadMesh(fileName);

		// Delete the old display list
		glDeleteLists(oldId, 1);
    }
	else if (key == FW_KEY_R)
	{
		if (rotating) {
			rotating = false;

			// Stop and reset the whole timer
			rotationTimer->unstart();
			rotationTimer->clearTotal();
		}
		else {
			rotating = true;
			rotationTimer->start();
		}
	}
	else if (key == FW_KEY_MOUSE_LEFT) {
		mouseLeftPressed = true;
		mousePrevCoord = mouseCurCoord = m_window.getMousePos();
	}
	else if (key == FW_KEY_MOUSE_RIGHT) {
		mouseRightPressed = true;
		mousePrevCoord = mouseCurCoord = m_window.getMousePos();
	}
	else if (key == FW_KEY_WHEEL_UP) 
	{
		cameraPos[2] -= 0.5f;
		//cerr << "Zooming in.." << endl;
	}
	else if (key == FW_KEY_WHEEL_DOWN) 
	{
		cameraPos[2] += 0.5f;
		//cerr << "Zooming out.." << endl;
	}
}

void App::handleKeyUp(const String& key) {
	if (key == FW_KEY_MOUSE_LEFT) {
		mouseLeftPressed = false;
	}
	else if (key == FW_KEY_MOUSE_RIGHT) {
		mouseRightPressed = false;
	}
}

//------------------------------------------------------------------------

// Initialize OpenGL's rendering modes
void App::initRendering()
{
    glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);       // Turn on light #0.
}

//------------------------------------------------------------------------

void FW::init(void)
{
    new App;
}

//------------------------------------------------------------------------
