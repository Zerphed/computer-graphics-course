#include "curve.h"
#include "extra.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
using namespace std;
using namespace FW;

namespace Base {
	static Mat4f bezierBase = makeMat4f(1.0, -3.0,  3.0, -1.0,
									    0.0,  3.0, -6.0,  3.0,
										0.0,  0.0,  3.0, -3.0,
										0.0,  0.0,  0.0,  1.0);
	
	static Mat4f bsplineBase = makeMat4f(1.0/6.0, -3.0/6.0,  3.0/6.0, -1.0/6.0,
				   			             4.0/6.0,  0.0/6.0, -6.0/6.0,  3.0/6.0,
										 1.0/6.0,  3.0/6.0,  3.0/6.0, -3.0/6.0,
										 0.0/6.0,  0.0/6.0,  0.0/6.0,  1.0/6.0);

	static Mat4f catmullromBase = makeMat4f(-1.0,  3.0, -3.0,  1.0,
											 2.0, -5.0,  4.0, -1.0,
										    -1.0,  0.0,  1.0,  0.0,
											 0.0,  2.0,  0.0,  0.0);
}

namespace
{
    // Approximately equal to.  We don't want to use == because of
    // precision issues with floating point.
    inline bool approx( const Vec3f& lhs, const Vec3f& rhs )
    {
        const float eps = 1e-8f;
        return ( lhs - rhs ).lenSqr() < eps;
    }

    // This is the core routine of the curve evaluation code.  Unlike
    // evalBezier, this is only designed to work on 4 control points.
    // Furthermore, it requires you to specify an initial binormal
    // Binit, which is iteratively propagated throughout the curve as
    // the curvepoints are generated.  Any other function that creates
    // cubic splines can use this function by a corresponding change
    // of basis.
    Curve coreBezier( const Vec3f& p0,
					  const Vec3f& p1,
					  const Vec3f& p2,
					  const Vec3f& p3,
                      const Vec3f& Binit,
                      unsigned steps )
    {
    
        // Control points into geometry matrix
        Mat4f G = makeMat4f(p0.x, p1.x, p2.x, p3.x,
							p0.y, p1.y, p2.y, p3.y,
							p0.z, p1.z, p2.z, p3.z,
							0.0,   0.0,  0.0,  0.0);

        // Return value
        Curve R( steps + 1 );

        // Step through t = i / steps.
        for( unsigned i = 0; i <= steps; ++i )
        {
            float t = float( i ) / steps;

			Vec4f powerBasis(1.0, t, t*t, t*t*t);
			Vec4f d_powerBasis(0.0, 1.0, 2*t, 3.0*t*t);

            // Evaluate the point P(t), and local coordinate system 
            // of curve at t. Store result at R[i].

            // If t = 0 (i = 0) we have to use Binit to ensure
            // that the local coordinate system is continuous at
            // conjunction points, where two splines meet.

			// V_i = q(t_i)
			R[i].V = (G*Base::bezierBase*powerBasis).getXYZ();
			// T_i = q'(t_i).normalized()
			R[i].T = ((G*Base::bezierBase*d_powerBasis).getXYZ()).normalized();

			// N_i = (B_i-1 X T_i).normalized()
            if( i == 0 )
				R[i].N = (Binit.cross(R[i].T)).normalized();
            else
				R[i].N = (R[i-1].B.cross(R[i].T)).normalized();

			// B_i = (T_i X N_i).normalized()
			R[i].B = (R[i].T.cross(R[i].N)).normalized();
        }

        return R;
    }
}
    

Curve evalBezier( const vector< Vec3f >& P, unsigned steps )
{
    // Check
    if( P.size() < 4 || P.size() % 3 != 1 )
    {
        cerr << "evalBezier must be called with 3n+1 control points." << endl;
        exit( 0 );
    }

    cerr << "\t>>> evalBezier has been called with the following input:" << endl;

    cerr << "\t>>> Control points (type vector< Vec3f >): "<< endl;
    for( unsigned i = 0; i < P.size(); ++i )
    {
        cerr << "\t>>> "; printTranspose(P[i]); cerr << endl;
    }

    cerr << "\t>>> Steps (type steps): " << steps << endl;

    // Return value: initialize with starting values.  Note the
    // initial normal of [0,1,0] will result in the
    // "counterclockwise" specification for 2D curves on the xy-plane.
    // I.e., the normal of the curve is facing left from the
    // direction of travel.  For 3D curves, the choice of direction is
    // kind of arbitrary. so this initial normal works too.
	vector< Vec3f >::const_iterator segmentStart = P.begin();
	vector< Vec3f >::const_iterator segmentEnd = P.size() > 4 ?
		P.begin() + 4 :
		P.end();

    Curve R
	(
		coreBezier
		(
			P[0], P[1], P[2], P[3],
            Vec3f( 0, 0, 1 ),
            steps
		)
	);

    // Build the rest of the curve
    for( unsigned i = 0; i < ( P.size() - 4 ) / 3; ++i )
    {
		Vec3f p0 = P[ 3 * i + 3 ];
		Vec3f p1 = P[ 3 * i + 4 ];
		Vec3f p2 = P[ 3 * i + 5 ];
		Vec3f p3 = P[ 3 * i + 6 ];
		
        vector< CurvePoint > Rnew
		(
            coreBezier
			(
                p0, p1, p2, p3,
                R.back().B, // init with previous computed binormal
				steps
			)
		);

        R.pop_back();
        R.insert( R.end(), Rnew.begin(), Rnew.end() );
    }

    // Finally, check for loop and patch ends
    if( approx( R.front().V, R.back().V ) &&
        approx( R.front().T, R.back().T ) &&
        !approx( R.front().N, R.back().N ) 
        )
    {
        Vec3f axis = R.front().N.cross(R.back().N);
        float angle = FW::acos( dot( R.front().N, R.back().N ) );
        float sign = ( dot( axis, R.front().T ) > 0 ) ? 1.f : -1.f;

        for( unsigned i = 0; i < R.size(); ++i )
        {
            float d = float( i ) / ( R.size() - 1 );
            Mat3f M = Mat3f::rotation( R[i].T, -sign * d * angle );

            R[i].N = M * R[i].N;
            R[i].B = M * R[i].B;
        }
    }

    return R;
    
}

Curve evalBspline( const vector< Vec3f >& P, unsigned steps )
{
    // Check
    if( P.size() < 4 )
    {
        cerr << "evalBspline must be called with 4 or more control points." << endl;
        exit( 0 );
    }

    cerr << "\t>>> evalBSpline has been called with the following input:" << endl;

    cerr << "\t>>> Control points (type vector< Vec3f >): "<< endl;
    for( unsigned i = 0; i < P.size(); ++i )
    {
        cerr << "\t>>> "; printTranspose(P[i]); cerr << endl;
    }

	
    cerr << "\t>>> Steps (type steps): " << steps << endl;

	// Use your evalBezier function to evaluate the B-spline.
    // Convert the input control points to suitable coordinate system.
    // See lecture notes for further instructions.
    vector<Vec3f> Pbz;

	for (int i = 0; i < P.size()-3; i++) {

		// Create the G matrix for conversion
		Mat4f G;

		for (int j = 0; j < 4; j++) {
			G.setCol(j, Vec4f(P[i+j].x, P[i+j].y, P[i+j].z, 0.0));
		}

		// Calculate the conversion matrix: G_2 = G_1 * B_1 * (B_2)^-1
		G = G * Base::bsplineBase * Base::bezierBase.inverted();

		// Detach the converted control points from the G matrix
		for (int j = 0; j < 3; j++) {
			Pbz.push_back(Vec3f(G.getCol(j)[0], G.getCol(j)[1], G.getCol(j)[2]));
		}

		// Only add the last column for the very last index
		if (i+3 == P.size()-1) {
			Pbz.push_back(Vec3f(G.getCol(3)[0], G.getCol(3)[1], G.getCol(3)[2]));
		}
	}

    return evalBezier(Pbz, steps);
}

Curve evalCircle( float radius, unsigned steps )
{
    // This is a sample function on how to properly initialize a Curve
    // (which is a vector< CurvePoint >).
    
    // Preallocate a curve with steps+1 CurvePoints
    Curve R( steps+1 );

    // Fill it in counterclockwise
    for( unsigned i = 0; i <= steps; ++i )
    {
        // step from 0 to 2pi
        float t = 2.0f * M_PI * float( i ) / steps;

        // Initialize position
        // We're pivoting counterclockwise around the y-axis
        R[i].V = radius * Vec3f( FW::cos(t), FW::sin(t), 0 );
        
        // Tangent vector is first derivative
        R[i].T = Vec3f( -FW::sin(t), FW::cos(t), 0 );
        
        // Normal vector is second derivative
        R[i].N = Vec3f( -FW::cos(t), -FW::sin(t), 0 );

        // Finally, binormal is facing up.
        R[i].B = Vec3f( 0, 0, 1 );
    }

    return R;
}

void drawCurve( const Curve& curve, float framesize )
{
    // Save current state of OpenGL
    glPushAttrib( GL_ALL_ATTRIB_BITS );

    // Setup for line drawing
    glDisable( GL_LIGHTING ); 
    glColor4f( 1, 1, 1, 1 );
    glLineWidth( 1 );
    
    // Draw curve
    glBegin( GL_LINE_STRIP );
    for( unsigned i = 0; i < curve.size(); ++i )
    {
        glVertex( curve[ i ].V );
    }
    glEnd();

    glLineWidth( 1 );

    // Draw coordinate frames if framesize nonzero
    if( framesize != 0.0f )
    {
        Mat4f M;

        for( unsigned i = 0; i < curve.size(); ++i )
        {
            M.setCol( 0, Vec4f( curve[i].N, 0 ) );
            M.setCol( 1, Vec4f( curve[i].B, 0 ) );
            M.setCol( 2, Vec4f( curve[i].T, 0 ) );
            M.setCol( 3, Vec4f( curve[i].V, 1 ) );

            glPushMatrix();
            glMultMatrixf( M.getPtr() );
            glScaled( framesize, framesize, framesize );
            glBegin( GL_LINES );
            glColor3f( 1, 0, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( 1, 0, 0 );
            glColor3f( 0, 1, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( 0, 1, 0 );
            glColor3f( 0, 0, 1 ); glVertex3d( 0, 0, 0 ); glVertex3d( 0, 0, 1 );
            glEnd();
            glPopMatrix();
        }
    }
    
    // Pop state
    glPopAttrib();
}

