#include "surf.h"
#include "extra.h"
using namespace std;
using namespace FW;

#define PI 3.14159265359

namespace
{
    // This is a generic function that generates a set of triangle
    // faces for a sweeping a profile curve along "something".  For
    // instance, say you want to sweep the profile curve [01234]:
    //
    //   4     9     10
    //    3     8     11
    //    2 --> 7 --> 12 ----------[in this direction]--------->
    //    1     6     13 
    //   0     5     14
    //
    // Then the "diameter" is 5, and the "length" is how many times
    // the profile is repeated in the sweep direction.  This function
    // generates faces in terms of vertex indices.  It is assumed that
    // the indices go as shown in the picture (the first dia vertices
    // correspond to the first repetition of the profile curve, and so
    // on).  It will generate faces [0 5 1], [1 5 6], [1 6 2], ...
    // The boolean variable "closed" will determine whether the
    // function closes the curve (that is, connects the last profile
    // to the first profile).
    static vector< Tup3u > triSweep( unsigned dia, unsigned len, bool closed )
    {
		// Indices: n, n+1, n+dia; n+1, n+dia+1, n+dia
		// max n == dia*(len-1)*2

        // Store the face indices in triplets (Tup3u), each triplet defining
        // one triangle.
        vector< Tup3u > ret;

		for (unsigned i = 0; i < len-1; i++) {
			for (unsigned j = i*dia; j < (dia*i) + (dia-1); j++) {
				ret.push_back(Tup3u(j, j+dia, j+1));
				ret.push_back(Tup3u(j+1, j+dia, j+dia+1));
			}
		}

		if (closed) {
			int j = 0;
			for (int i = dia*(len-1); i < dia*(len-1)+dia-1; i++, j++) {
				ret.push_back(Tup3u(i, j, i+1));
				ret.push_back(Tup3u(i+1, j, j+1));
			}
		}

        return ret;
    }

    
    // We're only implenting swept surfaces where the profile curve is
    // flat on the xy-plane.  This is a check function.
    static bool checkFlat(const Curve &profile)
    {
        for (unsigned i=0; i<profile.size(); i++)
            if (profile[i].V[2] != 0.0 || profile[i].T[2] != 0.0 || profile[i].N[2] != 0.0) {
				cerr << "V: " <<profile[i].V[2] << " T: " << profile[i].T[2] << " N: " << profile[i].N[2] << endl;
                return false;
			}
    
        return true;
    }
}

Surface makeSurfRev(const Curve &profile, unsigned steps)
{
    Surface surface;
    
    if (!checkFlat(profile))
    {
        cerr << "surfRev profile curve must be flat on xy plane." << endl;
        exit(0);
    }


    // Rotate the profile vertices around the y-axis [0, 1, 0]. 
    
    // For each slice
    for (unsigned i=0; i<steps; i++)
    {
        // Construct a 4x4 rotation transformation that rotates a point
        // around the y-axis an angle (2*PI*(i/steps)).

		Mat4f R = rotation4f(Vec3f(0.0, 1.0, 0.0), (-2.0*PI*(float(i)/float(steps))));

        // Recall that with a general transformation T you _would_ have
        // to transfrom normals with inverse transpose transformation,
        // but for a rotation matrix the inverse transpose happens 
        // to be the transformation itself.
        //
        // see rotation4f in extra.hpp

        // For each point on the profile
        for (unsigned j=0; j<profile.size(); j++)
        {
			Vec3f V = (R * Vec4f(profile[j].V.x, profile[j].V.y, profile[j].V.z, 1.0)).getXYZ();
			Vec3f N = (R * (-1.0) * Vec4f(profile[j].N.x, profile[j].N.y, profile[j].N.z, 1.0)).getXYZ();

			surface.VV.push_back(V);
			surface.VN.push_back(N);
        }
    }
    
    // Generate faces with triSweep -- the surface should be closed
	surface.VF = triSweep(profile.size(), steps, true);

    //cerr << "\t>>> makeSurfRev called (but not implemented).\n\t>>> Returning empty surface." << endl;

    return surface;
}

Surface makeGenCyl(const Curve &profile, const Curve &sweep )
{
    Surface surface;

    if (!checkFlat(profile))
    {
        cerr << "genCyl profile curve must be flat on xy plane." << endl;
        exit(0);
    }


    // Here you _should_ build the surface.  See surf.h for details.
    //
    // For each slice i and for each point j on profile:
    //
    // Consider a local coordinate system attached
    // to the point sweep[i].V in the sweep curve. The origin
    // of the coordinate system is sweep[j].V and the coordinate
    // axes are the normal, binormal, and tangent at that point
    // in the sweep curve (sweep[i].N, sweep[i].B, sweep[i].T).
    //
    // Transform the point profile[j].V and the normal
    // profile[j].N of the profile curve to the coordinate system 
    // described above.

	for (int i = sweep.size()-1; i >= 0; i--) {

		Mat4f M = makeMat4f(sweep[i].N.x, sweep[i].B.x, sweep[i].T.x, sweep[i].V.x,
							sweep[i].N.y, sweep[i].B.y, sweep[i].T.y, sweep[i].V.y,
							sweep[i].N.z, sweep[i].B.z, sweep[i].T.z, sweep[i].V.z,
							     0.0,          0.0,          0.0,           1.0);

		for (int j = 0; j < profile.size(); j++) {
			Vec3f V = (M * Vec4f(profile[j].V.x, profile[j].V.y, profile[j].V.z, 1.0)).getXYZ();
			Vec3f N = ((M.inverted().transposed() * (-1.0)) * Vec4f(profile[j].N.x, profile[j].N.y, profile[j].N.z, 1.0)).getXYZ();

			surface.VV.push_back(V);
			surface.VN.push_back(N);
			
		}
	}

    //cerr << "\t>>> makeGenCyl called (but not implemented).\n\t>>> Returning empty surface." <<endl;

	// Fill the face indices with triSweep.
	surface.VF = triSweep(profile.size(), sweep.size(), true);
    return surface;
}

void drawSurface(const Surface &surface, bool shaded)
{
    // Save current state of OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    if (shaded)
    {
        // This will use the current material color and light
        // positions.  Just set these in drawScene();
        glEnable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // This tells openGL to *not* draw backwards-facing triangles.
        // This is more efficient, and in addition it will help you
        // make sure that your triangles are drawn in the right order.
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else
    {        
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        glColor4f(0.4f,0.4f,0.4f,1.f);
        glLineWidth(1);
    }

    glBegin(GL_TRIANGLES);
    for (unsigned i=0; i<surface.VF.size(); i++)
    {
        glNormal(surface.VN[surface.VF[i][0]]);
        glVertex(surface.VV[surface.VF[i][0]]);
        glNormal(surface.VN[surface.VF[i][1]]);
        glVertex(surface.VV[surface.VF[i][1]]);
        glNormal(surface.VN[surface.VF[i][2]]);
        glVertex(surface.VV[surface.VF[i][2]]);
    }
    glEnd();

    glPopAttrib();
}

void drawNormals(const Surface &surface, float len)
{
    // Save current state of OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glDisable(GL_LIGHTING);
    glColor4f(0,1,1,1);
    glLineWidth(1);

    glBegin(GL_LINES);
    for (unsigned i=0; i<surface.VV.size(); i++)
    {
        glVertex(surface.VV[i]);
        glVertex(surface.VV[i] + surface.VN[i] * len);
    }
    glEnd();

    glPopAttrib();
}

void outputObjFile(ostream &out, const Surface &surface)
{
    
    for (unsigned i=0; i<surface.VV.size(); i++)
        out << "v  "
            << surface.VV[i][0] << " "
            << surface.VV[i][1] << " "
            << surface.VV[i][2] << endl;

    for (unsigned i=0; i<surface.VN.size(); i++)
        out << "vn "
            << surface.VN[i][0] << " "
            << surface.VN[i][1] << " "
            << surface.VN[i][2] << endl;

    out << "vt  0 0 0" << endl;
    
    for (unsigned i=0; i<surface.VF.size(); i++)
    {
        out << "f  ";
        for (unsigned j=0; j<3; j++)
        {
            unsigned a = surface.VF[i][j]+1;
            out << a << "/" << "1" << "/" << a << " ";
        }
        out << endl;
    }
}
