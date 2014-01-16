#pragma once
#include "gui/Window.hpp"
#include "gui/CommonControls.hpp"
#include "gpu/Buffer.hpp"

#include "camera.h"
#include "curve.h"
#include "surf.h"

#include <vector>
#include <string>

namespace FW
{
//------------------------------------------------------------------------

class App : public Window::Listener
{
private:
    enum Action
    {
        Action_None,
        Action_LoadSWP,
        Action_ResetView,
        Action_WriteOBJ
    };

    enum CullMode
    {
        CullMode_None = 0,
        CullMode_CW,
        CullMode_CCW,
    };

    enum CurveMode
    {
        CurveMode_None = 0,
        CurveMode_Draw,
        CurveMode_Normals
    };

    enum SurfaceMode
    {
        SurfaceMode_None = 0,
        SurfaceMode_Draw,
        SurfaceMode_WireFrame
    };

    enum PointMode
    {
        PointMode_False = 0,
        PointMode_True
    };

public:
                        App             (void);
    virtual             ~App            (void);

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
    CurveMode           m_curveMode;
    SurfaceMode         m_surfaceMode;
    PointMode           m_pointMode;

private:

    // 
    void arcballRotation(int endX, int endY);
    void keyDownFunc    (const Window::Event& ev);
    void keyUpFunc      (const Window::Event& ev);
    void motionFunc     (const Window::Event& ev);
    void reshapeFunc    (int w, int h);
    void drawScene      (void);
    void initRendering  (void);
    void loadObjects    (const std::string& fileName);
    void writeObjects   (const std::string& fileName);
    void makeDisplayLists(void);
    void screenshot     (const String& name);
    
private:
    
    // This is the camera
    Camera              m_camera;

    // These are state variables for the UI
    bool                m_mousePressed;

    // This detemines how big to draw the normals
    const float         m_lineLen;

    // These are arrays for display lists for each drawing mode.  The
    // convention is that drawmode 0 is "blank", and other drawmodes
    // just call the appropriate display lists.
    GLuint              m_curveLists[3];
    GLuint              m_surfaceLists[3];
    GLuint              m_axisList;
    GLuint              m_pointList;

    // These STL Vectors store the control points, curves, and
    // surfaces that will end up being drawn.  In addition, parallel
    // STL vectors store the names for the curves and surfaces (as
    // given by the files).
    std::vector<std::vector<Vec3f> > m_ctrlPoints;
    std::vector<Curve>              m_curves;
    std::vector<std::string>        m_curveNames;
    std::vector<Surface>            m_surfaces;
    std::vector<std::string>        n_surfaceNames;
};

//------------------------------------------------------------------------
}
