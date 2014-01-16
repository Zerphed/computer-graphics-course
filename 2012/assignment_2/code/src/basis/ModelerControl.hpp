#ifndef MODELER_CONTROL_HPP
#define MODELER_CONTROL_HPP

struct ModelerControl
{
    ModelerControl();
    ModelerControl(const char *name, float minimum, float maximum,
                   float stepsize, float value);
    ModelerControl(const ModelerControl & o);
    ModelerControl & operator=(const ModelerControl & o);
    
    void SetVals(const char *name, float minimum, float maximum,
                 float stepsize, float value);
    
    char  m_name[128];
    float m_minimum;
    float m_maximum;
    float m_stepsize;
    float m_value;
};

#endif
