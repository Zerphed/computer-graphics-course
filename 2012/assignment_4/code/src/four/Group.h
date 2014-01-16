#ifndef GROUP_H
#define GROUP_H

#include <cassert>
#include <vector>

#include "Object3D.h"

class Group : public Object3D
{
public:

    Group();
    Group( int nObjects );
    virtual ~Group();

    int numObjects() const;
    Object3D* getObject( int i ) const;
    void addObject( int i, Object3D* obj );

	bool intersectShadows(const Ray& r, Hit& h, float tmin, float tmax) const;
    virtual bool intersect(const Ray& r, Hit& h, float tmin) const;
	
private:

    std::vector< Object3D* > m_objects;

};

#endif // GROUP_H
