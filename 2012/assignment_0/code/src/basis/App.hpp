#pragma once
#include "gui/Window.hpp"
#include "gpu/Buffer.hpp"
#include "gui/CommonControls.hpp"

#include <vector>

namespace FW
{
//------------------------------------------------------------------------

class App : public Window::Listener
{
private:
    enum Action
    {
        Action_None,
        Action_LoadMesh
    };

    enum CullMode
    {
        CullMode_None = 0,
        CullMode_CW,
        CullMode_CCW,
    };

public:
                        App             (void);
    virtual             ~App            (void);

    // In handleEvent we can check what kind of an event has happened
    // and react appropriately.
    virtual bool        handleEvent     (const Window::Event& ev);

private:


private:
                        App             (const App&); // forbidden
    App&                operator=       (const App&); // forbidden

private:
    Window              m_window;
    CommonControls      m_commonCtrl;

    Action              m_action;
    String              m_meshFileName;
    CullMode            m_cullMode;

private:
    void                renderFrame     (void);
    void                loadMesh        (const String& fileName);
    void                handleKeyDown   (const String& key);
	void				handleKeyUp		(const String& key);
    void                initRendering   (void);

	// Display list generation
	GLuint				createDL		(void);

    // These are convenience functions which allow us to call OpenGL 
    // methods on Vec3f objects
    inline void         glVertex        (const Vec3f &a) { glVertex3fv(a.getPtr()); }
    inline void         glNormal        (const Vec3f &a) { glNormal3fv(a.getPtr()); }


private:
	Vec3f				getArcballVec		(GLfloat x, GLfloat y);
	GLfloat				getRotationAngle	(const Vec3f& va, const Vec3f& vb);
	void				rotateModel			(void);
	void				rotateCamera		(void);


private:
    // List of points (3D vectors)
    std::vector<Vec3f>  m_vecv;

    // List of normals (also 3D vectors)
    std::vector<Vec3f>  m_vecn;

    // List of faces (indices into vecv and vecn)
    std::vector<std::vector<unsigned> > m_vecf;

	// Display list id
	GLuint DLid;

	// Color index, to allow changing colors. Used to index the diffColors array in the renderFrame
	GLuint colorIndex, prevColorIndex;
	bool changingColor;
	GLfloat currentColor[4];

	// Light coordinates, to allow moving the light
	GLfloat Lt0pos[4];

	// Camera coodinates, to allow moving the camera
	GLfloat cameraPos[3];

	// Mouse coordinates and flags to allow mouse manipulation
	Vec2i mouseCurCoord;
	Vec2i mousePrevCoord;

	bool mouseLeftPressed;
	bool mouseRightPressed;

	// A flag to see whether the model is rotating due to pressed r key
	bool rotating;
	GLuint rotatedAngle;

	// Timer for the rotation
	Timer* rotationTimer;

	// Timer for the color change
	Timer* colorTimer;
};

//------------------------------------------------------------------------
}
