#ifndef MESH_H
#define MESH_H

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>


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

typedef std::pair<unsigned int, unsigned int> Edge;

// For edgeMap
struct EdgeHash {
    template<class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;
    }
};

struct EdgeEqual {
    template<class T1, class T2>
    bool operator()(const std::pair<T1, T2> &lhs, const std::pair<T1, T2> &rhs) const {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }
};

class Mesh{
private:
    bool textureBufferAlreadyCreated;

    void createWireframeIndicies();
public:
    GLuint VAO, VBO, EBO, NBO, UVBO, diffuse_texture_id, float_texture_id, VAO_wireframe, VBO_wireframe, EBO_wireframe;
    GLuint ambiant_bo, diffuse_bo, specular_bo, mra_bo, useTexture_bo;
    GLuint diffuse_texture_LOC, float_texture_LOC;
    GLuint edgeTexture_id, edgeTexture_LOC;
    std::string object_path, name;

    bool visible = true;

    std::vector <glm::vec3> vertices;
    std::vector <glm::vec3> normals;
    std::vector <glm::vec3> selectedEdges;
    std::vector<Tri> triangle_indicies;
    std::vector <unsigned int> indices;
    std::vector <unsigned int> wireframeLineIndicies;
    std::vector<uint32_t> displayedEdges_fragmentWireframe;
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
