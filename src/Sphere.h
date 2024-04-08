#ifndef Sphere_H
#define Sphere_H

#include "Object3D.h"
#include <cmath>

static glm::vec3 SphericalCoordinatesToEuclidean( glm::vec3 ThetaPhiR ) {
    return ThetaPhiR[2] * glm::vec3( cos(ThetaPhiR[0]) * cos(ThetaPhiR[1]) , sin(ThetaPhiR[0]) * cos(ThetaPhiR[1]) , sin(ThetaPhiR[1]) );
}
static glm::vec3 SphericalCoordinatesToEuclidean( float theta , float phi ) {
    return glm::vec3( cos(theta) * cos(phi) , sin(theta) * cos(phi) , sin(phi) );
}

static glm::vec3 EuclideanCoordinatesToSpherical( glm::vec3 xyz ) {
    float R = xyz.length();
    float phi = asin( xyz[2] / R );
    float theta = atan2( xyz[1] , xyz[0] );
    return glm::vec3( theta , phi , R );
}

class Sphere : public Object3D {
public:
    glm::vec3 m_center;
    float m_radius;

    Sphere() : Object3D() {}
    Sphere(glm::vec3 c , float r) : Object3D() , m_center(c) , m_radius(r) {}


};
#endif
