#ifndef PLANE_H
#define PLANE_H

#include "Object3D.h"

class Plane : public Object3D{
private:
    glm::vec3 m_center , m_normal;
public:
    Plane() : Object3D() {}
    Plane( glm::vec3 const & c , glm::vec3 const & n ) {
        m_center = c;
        m_normal = n;
        m_normal = glm::normalize(m_normal);
    }
    void setCenter( glm::vec3 const & c ) { m_center = c; }
    void setNormal( glm::vec3 const & n ) { m_normal = glm::normalize(n); }
    glm::vec3 const & center() const { return m_center; }
    glm::vec3 const & normal() const { return m_normal; }
};
#endif
