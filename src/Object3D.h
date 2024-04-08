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

    void applyTransform(){
        mesh->applyTransform(transform);
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

    glm::vec3 getPosition()
    {
        return glm::vec3 (0.0f);
    }

    void rotate(Axe axe, float angle)
    {
        glm::vec3 axis;
        switch (axe) {
            case Axe::X:
                axis = glm::vec3(1.0f, 0.0f, 0.0f);
                break;
            case Axe::Y:
                axis = glm::vec3(0.0f, 1.0f, 0.0f);
                break;
            case Axe::Z:
                axis = glm::vec3(0.0f, 0.0f, 1.0f);
                break;
            default:
                axis = glm::vec3(1.0f, 0.0f, 0.0f); // Default to X-axis rotation
                break;
        }

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
