#ifndef SQUARE_H
#define SQUARE_H
#include "Object3D.h"
#include <cmath>

class Square : public Object3D {
public:
    glm::vec3 m_normal;
    glm::vec3 m_bottom_left;
    glm::vec3 m_right_vector;
    glm::vec3 m_up_vector;

    Square() : Object3D() {}
    Square(glm::vec3 const & bottomLeft , glm::vec3 const & rightVector , glm::vec3 const & upVector , float width=1. , float height=1. ,
           float uMin = 0.f , float uMax = 1.f , float vMin = 0.f , float vMax = 1.f) : Object3D() {}

    void setQuad( glm::vec3 const & bottomLeft , glm::vec3 const & rightVector , glm::vec3 const & upVector , float width=1. , float height=1. ,
                  float uMin = 0.f , float uMax = 1.f , float vMin = 0.f , float vMax = 1.f) {
        m_right_vector = rightVector;
        m_up_vector = upVector;
        m_normal = glm::cross(rightVector , upVector);
        m_bottom_left = bottomLeft;

        m_normal = glm::normalize(m_normal);
        m_right_vector = glm::normalize(m_right_vector);
        m_up_vector = glm::normalize(m_up_vector);

        m_right_vector = m_right_vector*width;
        m_up_vector = m_up_vector*height;

        //TODO init le mesh de quad
    }
};
#endif // SQUARE_H
