#include "ModelerControl.hpp"

#include <cstring>
#include <cstdio>

ModelerControl::ModelerControl():m_minimum(0.0f), m_maximum(1.0f), m_stepsize(0.1f),
m_value(0.0f)
{
}

ModelerControl::ModelerControl(const char *name, float minimum,
			       float maximum, float stepsize, float value)
{
    SetVals(name, minimum, maximum, stepsize, value);
}

ModelerControl::ModelerControl(const ModelerControl & o)
{
    operator=(o);
}

ModelerControl & ModelerControl::operator=(const ModelerControl & o)
{
    if (this != &o)
	SetVals(o.m_name, o.m_minimum, o.m_maximum, o.m_stepsize,
		o.m_value);
    return *this;
}

void ModelerControl::SetVals(const char *name, float minimum,
			     float maximum, float stepsize, float value)
{
    strncpy(m_name, name, 128);
    m_minimum = minimum;
    m_maximum = maximum;
    m_stepsize = stepsize;
    m_value = value;
}
