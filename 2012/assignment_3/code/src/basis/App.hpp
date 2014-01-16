#pragma once
#include "gui/Window.hpp"
#include "gui/CommonControls.hpp"
#include "gpu/Buffer.hpp"

#include "camera.h"
#include "tuple.h"

#include <vector>
#include <string>

#include "particleSystem.h"
#include "integrator.h"


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
        Action_ResetView,
        Action_Shift_Left,
        Action_Shift_Right,
		Action_Emulate_Wind
    };

    enum CullMode
    {
        CullMode_None = 0,
        CullMode_CW,
        CullMode_CCW,
    };

    enum PolygonMode
    {
        PolygonMode_WireFrame,
        PolygonMode_Smooth
    };

    enum IntegratorMode
    {
        IntegratorMode_Euler,
        IntegratorMode_Trapezoidal,
		IntegratorMode_RungeKutta
    };

    enum ParticleSystemType
    {
        ParticleSystemType_Cloth,
        ParticleSystemType_Pendulum,
        ParticleSystemType_SimpleSystem
    };

public:
                        App             (void);
    virtual             ~App            (void);

    virtual bool        handleEvent     (const Window::Event& ev);

private:
                        App             (const App&); // forbidden
    App&                operator=       (const App&); // forbidden

private:

    // User interface stuff.
    Window              m_window;
    CommonControls      m_commonCtrl;
    Action              m_action;
    String              m_meshFileName;
    CullMode            m_cullMode;
    PolygonMode         m_polygonMode;

    // These are state variables for the UI
    bool                m_mousePressed;
    bool                m_drawAxes;

	bool                m_wind;

    IntegratorMode      m_integratorMode;
    bool                m_integratorChanged;

    ParticleSystemType  m_particleSystemType;
    bool                m_particleSystemChanged;

private:

    // The usual ui, input handling and setup functions.
    void arcballRotation(int endX, int endY);
    void keyDownFunc    (const Window::Event& ev);
    void keyUpFunc      (const Window::Event& ev);
    void motionFunc     (const Window::Event& ev);
    void reshapeFunc    (int w, int h);
    void drawScene      (void);
    void initRendering  (void);
    void writeObjects   (const std::string& fileName);
    void drawAxes       (void);

    void resetSystem    (ParticleSystemType particleSystemType, IntegratorMode integratorMode, int rows, int cols);
    void stepSystem     (void);
    void drawSystem     (void);

private:
    
    // This is the camera
    Camera              m_camera;

    ParticleSystem*     m_particleSystem;
    Integrator*         m_integrator;
    int                 m_interval; // step interval (milliseconds)
    float               m_stepSize;
    float               m_speed;
    Timer               m_timer;

};

//------------------------------------------------------------------------
}
