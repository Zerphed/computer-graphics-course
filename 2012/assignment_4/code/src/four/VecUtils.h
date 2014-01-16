#ifndef VEC_UTILS_H
#define VEC_UTILS_H

class VecUtils
{
public:

	static Mat4f rotate( const Vec3f& axis, float angle )
	{
		Mat3f R = Mat3f::rotation( axis, angle );
		Mat4f R4;
		R4.setIdentity();
		R4.setCol( 0, Vec4f( Vec3f(R.getCol(0)), 0.0f ) );
		R4.setCol( 1, Vec4f( Vec3f(R.getCol(1)), 0.0f ) );
		R4.setCol( 2, Vec4f( Vec3f(R.getCol(2)), 0.0f ) );
		return R4;
	}

    // transforms a 3D point using a matrix, returning a 3D point
    static Vec3f transformPoint( const Mat4f& mat, const Vec3f& point )
    {
        return ( mat * Vec4f( point, 1 ) ).getXYZ();
    }

    // transform a 3D directino using a matrix, returning a direction
    // This function *does not* take the inverse tranpose for you.
    static Vec3f transformDirection( const Mat4f& mat, const Vec3f& dir )
    {
        return ( mat * Vec4f( dir, 0 ) ).getXYZ();
    }
};

#endif // VEC_UTILS_H
