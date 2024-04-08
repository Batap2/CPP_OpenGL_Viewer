//
// Created by bat on 20/11/23.
//

#include "glm/glm.hpp"

#ifndef OBJECT_VIEWER_CAMERA_H
#define OBJECT_VIEWER_CAMERA_H

class Camera{

public:

    Camera(){};
    ~Camera(){};


    glm::vec3 cameraPos = glm::vec3(0,0.2,4);

    glm::vec3 cameraDirection = glm::vec3(0.0f,0.0f,-1.0);;

    glm::vec3 cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);


    // Fonction pour afficher la position de la caméra
    void printCameraPosition() const {
        std::cerr << "Camera Position: (" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")\n";
        std::cerr << "Camera Direction: (" << cameraDirection.x << ", " << cameraDirection.y << ", " << cameraDirection.z << ")\n";
        std::cerr << "Camera Right (" << cameraRight.x << ", " << cameraRight.y << ", " << cameraRight.z << ")\n";


    }

};

#endif //OBJECT_VIEWER_CAMERA_H
