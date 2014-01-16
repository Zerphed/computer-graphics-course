#pragma once
#include "gui/Window.hpp"
#include "gui/CommonControls.hpp"
#include "gpu/Buffer.hpp"

#include "camera.h"
#include "ModelerControl.hpp"
#include "SkeletalModel.h"
#include "tuple.h"

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
        Action_LoadModel,
        Action_ResetView
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

    virtual bool        handleEvent     (const Window::Event& ev);

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

    // 
    void arcballRotation(int endX, int endY);
    void keyDownFunc    (const Window::Event& ev);
    void keyUpFunc      (const Window::Event& ev);
    void motionFunc     (const Window::Event& ev);
    void reshapeFunc    (int w, int h);
    void drawScene      (void);
    void initRendering  (void);
    void loadModel      (const String& modelName);
    void writeObjects   (const std::string& fileName);
    void drawAxes       (void);

    void updateUI       (void);
    void updateJoints   (void);
    void updateModel    (void);
    
private:
    
    // This is the camera
    Camera              m_camera;

    // These are state variables for the UI
    bool                m_mousePressed;

    SkeletalModel       m_model;
    
    bool                m_drawSkeleton;
    bool                m_drawAxes;
    bool                m_drawJoints;

    float               m_x;
    float               m_y;
    float               m_z;
    bool                m_sliderChanged;

    std::vector< tuple<ModelerControl, 3> > m_controls;
    unsigned int        m_controlIndex;
    bool                m_jointChanged;
    
};

//------------------------------------------------------------------------
}
