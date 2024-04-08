#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object3D.h"

class Triangle : public Object3D{
private:
    glm::vec3 m_c[3] , m_normal;
    float area;
public:
    Triangle() : Object3D() {}
    Triangle( glm::vec3 const & c0 , glm::vec3 const & c1 , glm::vec3 const & c2 ) {
        m_c[0] = c0;
        m_c[1] = c1;
        m_c[2] = c2;
        updateAreaAndNormal();
    }
    void updateAreaAndNormal() {
        glm::vec3 nNotNormalized = glm::cross( m_c[1] - m_c[0] , m_c[2] - m_c[0] );
        float norm = glm::length(nNotNormalized);
        m_normal = nNotNormalized / norm;
        area = norm / 2.f;
    }
    void setC0( glm::vec3 const & c0 ) { m_c[0] = c0; } // remember to update the area and normal afterwards!
    void setC1( glm::vec3 const & c1 ) { m_c[1] = c1; } // remember to update the area and normal afterwards!
    void setC2( glm::vec3 const & c2 ) { m_c[2] = c2; } // remember to update the area and normal afterwards!

    glm::vec3 const & normal() const { return m_normal; }
};
#endif
