//
// Created by bat on 22/11/23.
//

#ifndef OBJECT_VIEWER_HITTABLE_H
#define OBJECT_VIEWER_HITTABLE_H

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include "Mesh.h"

enum Axe{
    X,Y,Z
};

class Object3D
{
public:
    Object3D(){
    }
    ~Object3D();

    Mesh* mesh;

    glm::mat4 transform = glm::mat4(1.0f);

    glm::vec3 position;
    glm::vec3 rotation;

    void applyTransform(){
        mesh->applyTransform(transform);
        position = getPosition();
    }

    glm::vec3 getRotationEulerAngles()
    {
        glm::quat rotationQuat(transform);
        return glm::eulerAngles(rotationQuat);
    }

    glm::mat3 getRotationMatrix()
    {
        return glm::mat3(transform);
    }

    void updatePosition()
    {
        transform[3][0] = position.x;
        transform[3][1] = position.y;
        transform[3][2] = position.z;
    }

    glm::vec3 getPosition()
    {
        return glm::vec3(transform[3][0], transform[3][1], transform[3][2]);
    }

    void rotate(glm::vec3 axis, float angle)
    {
        glm::quat rotationQuat = glm::angleAxis(glm::radians(angle), axis);
        transform = glm::mat4_cast(rotationQuat) * transform;
    }

    void translate(glm::vec3 vec)
    {
        transform = glm::translate(transform, vec);
    }

    void translate(Axe axe, float distance)
    {
        glm::vec3 translation(0.0f);
        switch (axe) {
            case Axe::X:
                translation.x = distance;
                break;
            case Axe::Y:
                translation.y = distance;
                break;
            case Axe::Z:
                translation.z = distance;
                break;
            default:
                break;
        }
        transform = glm::translate(transform, translation);
    }
};

#endif //OBJECT_VIEWER_HITTABLE_H
