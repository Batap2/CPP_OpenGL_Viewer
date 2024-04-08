//
// Created by bat on 20/11/23.
//

#ifndef OBJECT_VIEWER_MATERIAL_H
#define OBJECT_VIEWER_MATERIAL_H

#include <array>
#include "glm/glm.hpp"
enum MaterialType {
    Material_Diffuse_Blinn_Phong ,
    Material_Glass,
    Material_Mirror
};

enum LightingType {
    LightingType_Phong,
    LightingType_PBR
};

struct Texture{
    unsigned int height, width;
    std::vector<unsigned char> data;
};

struct FloatTexture{
        unsigned int height, width;
        std::vector<float> data;
};


struct Material {
    MaterialType mType;
    LightingType lType;

    float useTexture;
    float isEmissive;
    float isTransparent;

    glm::vec3 ambient_material;
    glm::vec3 diffuse_material;

    glm::vec3 specular_material;

    float ambiant_strength = 0.1f;
    float specular_strength = 0.2f;
    float emissive_intensity = 0.0f;

    Texture diffuse_texture;

    FloatTexture float_texture;
    float shininess;
    float IOR;

    float transparency;

    // ---------- LightingType_PBR ----------
    float metallic, roughness, ao;

    Material() {
        mType = Material_Diffuse_Blinn_Phong;
        lType = LightingType_Phong;
        transparency = 0.0;
        IOR = 1.0;
        ambient_material = glm::vec3(0.0f, 0.0f, 0.0f);
        ao = 10;

        roughness = 0.6f;
        metallic = 0.0f;

        useTexture = 0;
        isEmissive = 0;
        isTransparent = 0;
        emissive_intensity = 0;
    }

    std::array<float, 13> getMaterialData()
    {
        std::array<float, 13> arr;

        arr[0] = ambient_material[0];
        arr[1] = ambient_material[1];
        arr[2] = ambient_material[2];
        arr[3] = diffuse_material[0];
        arr[4] = diffuse_material[1];
        arr[5] = diffuse_material[2];
        arr[6] = specular_material[0];
        arr[7] = specular_material[1];
        arr[8] = specular_material[2];
        arr[9] = shininess;
        arr[10] = metallic;
        arr[11] = roughness;
        arr[12] = ao;

        return arr;
    }

    // metallic, roughness, ao
    glm::vec3 getMRA()
    {
        return glm::vec3(metallic,roughness,ao);
    }
};
#endif //OBJECT_VIEWER_MATERIAL_H
