//
// Created by bat on 20/11/23.
//

#include "glm/glm.hpp"

#ifndef OBJECT_VIEWER_LIGHT_H
#define OBJECT_VIEWER_LIGHT_H
class Light{
public:
    enum LightType {
        LightType_Spherical,
        LightType_Quad
    };


    Light(){
        intensity = 1;
        powerCorrection = 1;
        type = LightType_Spherical;
        pos = glm::vec3(0,0,0);
        color = glm::vec3(1,1,1);
    }
    ~Light(){};

    glm::vec3 color;
    bool isInCamSpace;

    LightType type;
    glm::vec3 pos;

    float radius;
    float intensity;

    //Square quad;

    float powerCorrection;

    std::vector<float> getLightData() {
        std::vector<float> lightData;

        // Ajouter les données de la lumière au vecteur
        type == LightType_Spherical ? lightData.push_back(0.0) : lightData.push_back(1.0) ;
        isInCamSpace? lightData.push_back(1.0) : lightData.push_back(0.0) ;
        lightData.push_back(pos.x);
        lightData.push_back(pos.y);
        lightData.push_back(pos.z);
        lightData.push_back(radius);
        lightData.push_back(intensity);
        lightData.push_back(powerCorrection);
        lightData.push_back(color.r);
        lightData.push_back(color.g);
        lightData.push_back(color.b);

        return lightData;
    }
};
#endif //OBJECT_VIEWER_LIGHT_H
