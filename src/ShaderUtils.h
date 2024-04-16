//
// Created by bat on 21/11/23.
//

#ifndef OBJECT_VIEWER_SHADERUTILS_H
#define OBJECT_VIEWER_SHADERUTILS_H

#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include "globals.h"
#include "shaders.h"


namespace ShaderUtils{

    struct ShaderLight{
        glm::vec3 pos;
        glm::vec3 color;
        float intensity;
    };

    template <typename F>
    void forAllShader(const F& func)
    {
        GLuint shaders[3] = {shaderProgram_main, shaderProgram_Flat, shaderProgram_NormalDisplay};
        for (auto& sha : shaders)
        {
            glUseProgram(sha);

            func(sha);
        }
        glUseProgram(shaderProgram_main);
    }

    //TODO  cpt
    void drawLights()
    {

//            glPointSize(5.0f); // Définit la taille du point
//
//            glBegin(GL_POINTS); // Commence à dessiner des points
//            glColor3f(l->color.x, l->color.y, l->color.z); // Couleur du point : blanc
//            glVertex3f(l->pos.x, l->color.y, l->color.z); // Coordonnées du point (ici, le centre de l'écran)
//            glEnd();

        GLuint lightsVertexArray, lightsVertexBuffer;

        glGenVertexArrays(1, &lightsVertexArray);
        glGenBuffers(1, &lightsVertexBuffer);

        glBindVertexArray(lightsVertexArray);

        // Bind vertices to layout location 0
        glBindBuffer(GL_ARRAY_BUFFER, lightsVertexArray );

        std::vector<glm::vec3> lightsVertex;
        for(Light* l : scene_lights){
            lightsVertex.push_back(l->pos);
        }

        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * lightsVertex.size(), &lightsVertex[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(50); // This allows usage of layout location 0 in the vertex shader
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(lightsVertexArray);

        glPointSize(2.5);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        glDrawElements(GL_TRIANGLES, scene_lights.size()*3, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

    }

    void init_shaders()
    {
        Shader shader;
        vertexShader_main = shader.init_shaders(GL_VERTEX_SHADER, "../res/shaders/vertex.glsl");

        geometryShader_main = shader.init_shaders(GL_GEOMETRY_SHADER, "../res/shaders/geometry_main.glsl");
        geometryShader_Flat = shader.init_shaders(GL_GEOMETRY_SHADER, "../res/shaders/geometry_Flat.glsl");
        geometryShader_NormalDisplay = shader.init_shaders(GL_GEOMETRY_SHADER, "../res/shaders/geometry_NormalDisplay.glsl");
        geometryShader_WireframeDisplay = shader.init_shaders(GL_GEOMETRY_SHADER, "../res/shaders/geometry_WireframeDisplay.glsl");

        fragmentShader_main = shader.init_shaders(GL_FRAGMENT_SHADER, "../res/shaders/fragment_main.glsl");
        fragmentShader_NormalDisplay = shader.init_shaders(GL_FRAGMENT_SHADER, "../res/shaders/fragment_NormalDisplay.glsl");
        fragmentShader_WireframeDisplay = shader.init_shaders(GL_FRAGMENT_SHADER, "../res/shaders/fragment_WireframeDisplay.glsl") ;


        shaderProgram_main = shader.init_program(vertexShader_main, geometryShader_main, fragmentShader_main) ;
        shaderProgram_Flat = shader.init_program(vertexShader_main, geometryShader_Flat, fragmentShader_main);
        shaderProgram_NormalDisplay = shader.init_program(vertexShader_main, geometryShader_NormalDisplay, fragmentShader_NormalDisplay);
        shaderProgram_WireframeDisplay = shader.init_program(vertexShader_main, geometryShader_WireframeDisplay, fragmentShader_WireframeDisplay);

        projectionLoc = glGetUniformLocation(shaderProgram_main, "projection");
        modelviewLoc = glGetUniformLocation(shaderProgram_main, "modelview");
        camPosLoc = glGetUniformLocation(shaderProgram_main, "camPos");

        projectionLocFS = glGetUniformLocation(shaderProgram_Flat, "projection");
        modelviewLocFS = glGetUniformLocation(shaderProgram_Flat, "modelview");
        camPosLocFS = glGetUniformLocation(shaderProgram_main, "camPos");

        projectionLocNS = glGetUniformLocation(shaderProgram_NormalDisplay, "projection");
        modelviewLocNS = glGetUniformLocation(shaderProgram_NormalDisplay, "modelview");

        projectionLocWS = glGetUniformLocation(shaderProgram_WireframeDisplay, "projection");
        modelviewLocWS = glGetUniformLocation(shaderProgram_WireframeDisplay, "modelview");
        screenSizeLoc = glGetUniformLocation(shaderProgram_WireframeDisplay, "screenSize");
        wireframeWidthLocWS = glGetUniformLocation(shaderProgram_WireframeDisplay, "wireframeWidth");
        glUseProgram(shaderProgram_WireframeDisplay);
        glUniform2f(screenSizeLoc, (float)window_width, (float)window_height);
        glUniform1f(wireframeWidthLocWS, wireframeWidth);

        glUseProgram(shaderProgram_NormalDisplay);
        normalDisplayLengthLoc = glGetUniformLocation(shaderProgram_NormalDisplay, "normalDisplayLength");
        glUniform1f(normalDisplayLengthLoc, normalDisplayLength);

        glUseProgram(shaderProgram_main);
    }

    void reshape(GLFWwindow* window, int width, int height){
        window_width = width;
        window_height = height;

        // vp pour avoir une rendu plus petit
        //int vp = width / screenSeparation1;
        int vp = 0;

        current_vp_height = height;
        current_vp_width = vp;

        glViewport(vp, 0, width - vp, height);

        float aspect = (float) (width - vp) / (float) height;
        projection = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);

        forAllShader(
                [](GLuint sha){
                    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
                });
    }

    void sendLightsToShaders()
    {
        if(scene_lights.size() > lightsMaxNumber){
            std::cout << "max scene_lights number reached\n";
            return;
        }

        int structSize = 7;
        GLfloat lightsBuffer[lightsMaxNumber * structSize];


        for(int i = 0; i < scene_lights.size(); ++i)
        {
            Light* l = scene_lights[i];

            int offset = i*structSize;

            lightsBuffer[offset] = l->pos.x;
            lightsBuffer[offset+1] = l->pos.y;
            lightsBuffer[offset+2] = l->pos.z;
            lightsBuffer[offset+3] = l->color.x;
            lightsBuffer[offset+4] = l->color.y;
            lightsBuffer[offset+5] = l->color.z;
            lightsBuffer[offset+6] = l->intensity;
        }

        lightsBufferID = glGetUniformLocation(shaderProgram_main, "lights");
        lights_numberID = glGetUniformLocation(shaderProgram_main, "lights_number");

        glUseProgram(shaderProgram_Flat);
        glUniform1fv(lightsBufferID, lightsMaxNumber * structSize, lightsBuffer);
        glUniform1i(lights_numberID, scene_lights.size());

        glUseProgram(shaderProgram_main);
        glUniform1fv(lightsBufferID, lightsMaxNumber * structSize, lightsBuffer);
        glUniform1i(lights_numberID, scene_lights.size());
    }

    void sendMaterialsToShader()
    {
        if(scene_lights.size() > lightsMaxNumber){
            std::cout << "max scene_lights number reached\n";
            return;
        }

        // struct :
        // [ambiant_color, diffuse_color, specular_color, shininess, metallic, roughness, ao]
        // [3,3,3,1,1,1,1]
        int structSize = 13;
        GLfloat materialBuffer[objectMaxNumber * structSize];

        for(int i = 0; i < scene_meshes.size(); ++i)
        {
            Material m = scene_meshes[i]->material;

            int offset = i*structSize;

            materialBuffer[offset] = m.ambient_material[0];
            materialBuffer[offset+1] = m.ambient_material[1];
            materialBuffer[offset+2] = m.ambient_material[2];

            materialBuffer[offset+3] = m.diffuse_material[0];
            materialBuffer[offset+4] = m.diffuse_material[1];
            materialBuffer[offset+5] = m.diffuse_material[2];

            materialBuffer[offset+6] = m.specular_material[0];
            materialBuffer[offset+7] = m.specular_material[1];
            materialBuffer[offset+8] = m.specular_material[2];

            materialBuffer[offset+9] = m.shininess;
            materialBuffer[offset+10] = m.metallic;
            materialBuffer[offset+11] = m.roughness;
            materialBuffer[offset+12] = m.ao;
        }


        materialBufferID = glGetUniformLocation(shaderProgram_main, "materials");
        lights_numberID = glGetUniformLocation(shaderProgram_main, "materials_number");

        glUseProgram(shaderProgram_Flat);
        glUniform1fv(materialBufferID, objectMaxNumber * structSize, materialBuffer);
        glUniform1i(objectNumberID, scene_meshes.size());

        glUseProgram(shaderProgram_main);
        glUniform1fv(materialBufferID, objectMaxNumber * structSize, materialBuffer);
        glUniform1i(objectNumberID, scene_meshes.size());
    }

    float calculMoyenne(float nouveau, float ancienneMoyenne, int nombreElementDansMoyenne) {
        return ((ancienneMoyenne * nombreElementDansMoyenne) + nouveau) / (nombreElementDansMoyenne + 1);
    }

    void concatRender(std::vector<float> & new_render){
        for(int n = 0; n < actual_render.size(); ++n){
            float importance = 1/(float)render_number;
            //actual_render[n] = (1-importance) * actual_render[n] + importance * new_render[n];
            //actual_render[n] = 0.8 * actual_render[n] + 0.2 * new_render[n];
            actual_render[n] = calculMoyenne(new_render[n], actual_render[n], render_number);

            // 1 + 2 + 3 + 4 / 4 = 2;
            // moy = (moy + 1)/1 = 1
            // moy = (moy + 2)/2 = 1.5
            // moy = (moy + 3)/3 = 1.5
            // moy = (moy + 4)/4 =
        }
    }
}

#endif //OBJECT_VIEWER_SHADERUTILS_H
