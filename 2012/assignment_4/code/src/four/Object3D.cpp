#include "Object3D.h"

#include <climits>

Object3D::Object3D() :
    m_material( NULL )
{

}

Object3D::Object3D( Material* m ) :
    m_material( m )
{

}

// virtual
Object3D::~Object3D()
{ 
}

// virtual
Material* Object3D::getMaterial() const
{
    return m_material;
}

void Object3D::setMaterial( Material* m )
{
    m_material = m;
}

