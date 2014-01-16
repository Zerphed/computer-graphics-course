#include "Group.h"

#include "Triangle.h"

Group::Group()
{

}

Group::Group( int nObjects ) :

    m_objects( nObjects, NULL )

{

}

// virtual
Group::~Group()
{
    for( unsigned i = 0; i < m_objects.size(); ++i )
    {
        delete m_objects[i];
    }
}

int Group::numObjects() const
{
    return m_objects.size();
}

Object3D* Group::getObject( int i ) const
{
    assert( i >= 0 && i < numObjects() );
    return m_objects[i];
}

void Group::addObject( int i, Object3D* obj )
{
    assert( obj != NULL );
    assert( i >= 0 && i < numObjects() );
    m_objects[i] = obj; 
}

bool Group::intersectShadows( const Ray& r, Hit& h, float tmin, float tmax) const
{
	for (unsigned i = 0; i < m_objects.size(); ++i)
	{
		if (m_objects[i]->intersect(r, h, tmin) && h.getT() < tmax) {
			return true;
		}
	}
	return false;
}

// virtual
bool Group::intersect( const Ray& r, Hit& h, float tmin) const
{
	/*** YOUR CODE HERE
	     You should intersect the ray with each object contained in the group.
	**/
	bool intersected = false;

	for (unsigned i = 0; i < m_objects.size(); ++i)
	{
		if (m_objects[i]->intersect(r, h, tmin)) {
			intersected = true;
		}
	}

	return intersected;
}
