
#include "extra.h"
#include "simpleSystem.h"

using namespace std;
using namespace FW;

SimpleSystem::SimpleSystem():ParticleSystem(1)
{
	this->numParticles = 1;

    // Initial state of the one particle system.
	state = vector<Vec3f>(1);
	state[0] = Vec3f(0,1,0);

    // Radius of trajectory for visualization.
    r = state[0].length();
}

// for a given state, evaluate f(X,t)
vector<Vec3f> SimpleSystem::evalF(vector<Vec3f> state)
{
    // This function is called by your integrator.
    // The integrator provides the vector<Vec3f> of states.
    // Ofcourse, for this one particle system, state has only
    // one Vec3f.
	//
	// f(X, t) = (-y, x, 0), where X = (x, y, z)
	for (size_t i = 0; i < state.size(); i++) {
		state[i] = FW::Vec3f(-state[i].y, state[i].x, 0.0f);
	}

    return state;
}

// render the system (ie draw the particles)
void SimpleSystem::draw()
{
    // Draws the particle at the current position.
    Vec3f pos = state[0];
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2] );
    glutSolidSphere(0.075f,10.0f,10.0f);
    glPopMatrix();

    // Visualize the perfect trajectory.
    glLineWidth(3);
    float step = 2.0f * M_PI / 360.0;
    for (int i = 1; i < 361; ++i)
    {
        float x1 = r * FW::cos(step * (float)(i-1));
        float y1 = r * FW::sin(step * (float)(i-1));
        float z1 = 0.0f;

        float x2 = r * FW::cos(step * (float)i);
        float y2 = r * FW::sin(step * (float)i);
        float z2 = 0.0f;

        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex3f(x1, y1, z1);
        glVertex3f(x2, y2, z2);
        glEnd();
    }
}

void SimpleSystem::shiftSystem(bool forward) 
{
    // This function is not meaningful for a one particle system,
    // so you should leave this empty.
}
