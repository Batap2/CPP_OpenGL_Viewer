//
// Created by bat on 20/11/23.
//

#include "glm/glm.hpp"

#ifndef OBJECT_VIEWER_CAMERA_H
#define OBJECT_VIEWER_CAMERA_H

class Camera{

public:

    Camera(glm::vec3 pos, glm::vec3 direction){
        cameraPos = pos;
        cameraDirection = direction;
    };
    ~Camera(){};


    glm::vec3 cameraPos;
    glm::vec3 cameraDirection;

    glm::vec3 cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    void updatePos(float pitch, float yaw)
    {
        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        cameraDirection = glm::normalize(direction);
        cameraRight = glm::normalize(glm::cross(cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
        cameraUp = glm::cross(cameraRight, cameraDirection);
    }
};

#endif //OBJECT_VIEWER_CAMERA_H
