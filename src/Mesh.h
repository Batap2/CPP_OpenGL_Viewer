#ifndef MESH_H
#define MESH_H

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

#include "glm/glm.hpp"
#include "Material.h"

struct Tri{
    uint32_t vertices[3];
    Tri(uint32_t a, uint32_t b, uint32_t c){
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
    }
    Tri(){
        vertices[0] = 0;
        vertices[1] = 0;
        vertices[2] = 0;
    }
};

class Mesh{
private:
    bool textureBufferAlreadyCreated;
public:
    GLuint VAO, VBO, EBO, NBO, UVBO, diffuse_texture_id, float_texture_id;
    GLuint ambiant_bo, diffuse_bo, specular_bo, mra_bo, useTexture_bo;
    GLuint diffuse_texture_LOC, float_texture_LOC;
    std::string object_path, name;

    std::vector <glm::vec3> vertices;
    std::vector <glm::vec3> normals;
    std::vector <glm::vec3> selectedEdges;
    std::vector<Tri> triangle_indicies;
    std::vector <unsigned int> indicies;
    std::vector<float> uv;

    Material material;

    glm::vec3 bbmin;
    glm::vec3 bbmax;

    void applyTransform(glm::mat4 transform);

    void openglInit();

    void destroy_buffers();

    void change_texture(FloatTexture tex);

    void send_material_to_shaders();

    void updateMaterial();
};

#endif
