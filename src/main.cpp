#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/ext.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


#include "Mesh.h"
#include "camera.h"
#include "GUI.h"
#include "ShaderUtils.h"
#include "SceneOperations.h"

#include "shaders.h"
#include "transform.h"
#include "globals.h"


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
        rclickHolded = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE){
        rclickHolded = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS){
        switch(key){
            case GLFW_KEY_ESCAPE: 
                exit(0) ;
                break ;
            case GLFW_KEY_Y:
                mainCamera.cameraPos = eyeinit ;
                mainCamera.cameraUp = upinit ;
                amount = amountinit ;
                transop = view2 ;
                sx = sy = 1.0 ; 
                tx = ty = 0.0 ;

                break ;
            case GLFW_KEY_A:
                leftHolded = true;
                break;
            case GLFW_KEY_D:
                rightHolded = true;
                break;
            case GLFW_KEY_W:
                upHolded = true;
                break;
            case GLFW_KEY_S:
                 downHolded = true;
                break;
            case GLFW_KEY_Q:
                aHolded = true;
                break;
            case GLFW_KEY_E:
                eHolded = true;
                break;
            case GLFW_KEY_TAB:
                showMenus = !showMenus;
                break;
            case GLFW_KEY_N:
                displayNormals = !displayNormals;
                break;
            case GLFW_KEY_T:
                render_mode = (render_mode + 1)%2;
                //glUniform1i(render_modeLoc, render_mode);
                break;
            case GLFW_KEY_P:
                flat_screen.change_texture(FloatTexture());
                break;
            case GLFW_KEY_O:
                scene_objects[0]->translate(vec3(1,0,0));
                scene_objects[0]->applyTransform();
                break;
        }
    } else if (action == GLFW_RELEASE)
    {
        switch(key){
            case GLFW_KEY_A:
                leftHolded = false;
                break;
            case GLFW_KEY_D:
                rightHolded = false;
                break;
            case GLFW_KEY_W:
                upHolded = false;
                break;
            case GLFW_KEY_S:
                downHolded = false;
                break;
            case GLFW_KEY_Q:
                aHolded = false;
                break;
            case GLFW_KEY_E:
                eHolded = false;
                break;
        }
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(rclickHolded)
    {
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        mainCamera.updatePos(pitch, yaw);
        render_number = 0;

    } else {
        lastX = xpos;
        lastY = ypos;
    }

    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){

    if(yoffset > 0)
        cameraSpeed *= 1.25;
    if(yoffset < 0)
        cameraSpeed /= 1.25;
}

void manageInput()
{
    if(rclickHolded)
    {
        render_number = 0;
        if(leftHolded){
            mainCamera.cameraPos -= cameraSpeed * mainCamera.cameraRight;
        }
        if(rightHolded){
            mainCamera.cameraPos += cameraSpeed * mainCamera.cameraRight;
        }
        if(upHolded){
            mainCamera.cameraPos += cameraSpeed * mainCamera.cameraDirection;
        }
        if(downHolded){
            mainCamera.cameraPos -= cameraSpeed * mainCamera.cameraDirection;
        }
        if(aHolded){
            mainCamera.cameraPos -= cameraSpeed * upinit;
        }
        if(eHolded){
            mainCamera.cameraPos += cameraSpeed * upinit;
        }
    }
}

void display() {

    uint32_t meshCount = 1;

    glClearColor(skyColor.x, skyColor.y, skyColor.z, 0);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


    glStencilMask(0xFF);

    modelview = glm::lookAt(mainCamera.cameraPos,mainCamera.cameraPos + mainCamera.cameraDirection, mainCamera.cameraUp);

    GLuint usedShader;

    if(render_mode == 0){
        usedShader = shaderProgram_main;
        glUseProgram(shaderProgram_main);
        glUniform3fv(camPosLoc, 1, glm::value_ptr(mainCamera.cameraPos));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, &modelview[0][0]);
    } else if(render_mode == 1){
        usedShader = shaderProgram_Flat;
        glUseProgram(shaderProgram_Flat);
        glUniform3fv(camPosLocFS, 1, glm::value_ptr(mainCamera.cameraPos));
        glUniformMatrix4fv(projectionLocFS, 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(modelviewLocFS, 1, GL_FALSE, &modelview[0][0]);
    }

    if(wireframeMode)
    {
        usedShader = shaderProgram_BarycentricWireframe;
        glUseProgram(usedShader);
        glUniform3fv(camPosLocBWS, 1, glm::value_ptr(mainCamera.cameraPos));
        glUniformMatrix4fv(projectionLocBWS, 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(modelviewLocBWS, 1, GL_FALSE, &modelview[0][0]);

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(skyColor.x, skyColor.y, skyColor.z, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    if(wireframeMode == 2)
    {
        glUseProgram(shaderProgram_WireframeDisplay);
        glUniformMatrix4fv(projectionLocWS, 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(modelviewLocWS, 1, GL_FALSE, &modelview[0][0]);
    }


    if(displayNormals)
    {
        glUseProgram(shaderProgram_NormalDisplay);
        glUniformMatrix4fv(projectionLocNS, 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(modelviewLocNS, 1, GL_FALSE, &modelview[0][0]);
    }

    auto renderMesh = [&](Mesh* meshP){
        glUseProgram(usedShader);

        glStencilFunc(GL_ALWAYS, meshCount, 0xFF);

        glBindVertexArray(meshP->VAO);

        if(meshP->visible)
        {
            glActiveTexture(GL_TEXTURE0 + 0);
            glBindTexture(GL_TEXTURE_2D, meshP->diffuse_texture_id);

            glActiveTexture(GL_TEXTURE0 + 1);
            glBindTexture(GL_TEXTURE_1D, meshP->edgeTexture_id);

            glDrawElements(GL_TRIANGLES, meshP->indices.size(), GL_UNSIGNED_INT, 0);
        }

        if(displayNormals)
        {
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_STENCIL_TEST);
            glUseProgram(shaderProgram_NormalDisplay);
            glDrawElements(GL_TRIANGLES, meshP->indices.size(), GL_UNSIGNED_INT, 0);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_STENCIL_TEST);
        }

        meshCount = (meshCount % 256) + 1;
    };

    if(wireframeMode == 2)
    {
        std::vector<Mesh*> orderedMeshes;

        SceneOperations::orderMeshes(orderedMeshes);

        for(Mesh* meshP : orderedMeshes)
        {
            renderMesh(meshP);
        }

    } else
    {
        for(Mesh* meshP : scene_meshes)
        {
            renderMesh(meshP);
        }
    }

    if(wireframeMode)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glUseProgram(shaderProgram_frameBufferWireframe);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthBuffer);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        glBindVertexArray(frameBufferQuadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
    }

    if(wireframeMode == 2)
    {
        glDisable(GL_DEPTH_TEST);
        glUseProgram(shaderProgram_WireframeDisplay);
        for(Mesh* meshP : scene_meshes) {
            glBindVertexArray(meshP->VAO_wireframe);
            glDrawElements(GL_LINES, meshP->wireframeLineIndicies.size(), GL_UNSIGNED_INT, 0);
        }
        glEnable(GL_DEPTH_TEST);
    }


}

int main(int argc, char* argv[]){

    // Initialise GLFW and GLEW;
    GLFWwindow* window;

    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    window = glfwCreateWindow(window_width, window_height, "Scene Viewer", NULL, NULL);
    glfwMakeContextCurrent(window);
    // 1800 fps
    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK) std::cout << "ERROR : glewInit\n ";
    std::cout << glGetString(GL_VERSION) << "\n";

    // Initialize ImGui
    const char* glsl_version = "#version 330";
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::StyleColorsDark();

    // Initialise all variable initial values
    ShaderUtils::init_shaders();

    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);


    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSetWindowSizeCallback(window, ShaderUtils::reshape);

    glfwSetKeyCallback(window, keyboard);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    mainCamera.updatePos(pitch, yaw);

    SceneOperations::initSceneLights();
//    SceneOperations::openFile("../data/bunny.obj");
//
    //SceneOperations::openFile("../data/doon.obj");
//    scene_objects[0]->translate(glm::vec3(0,0,-1.2));
//    scene_objects[0]->applyTransform();

    //scene_objects[0]->rotate(vec3(0,1,1), 0.1);
    //SceneOperations::openFile("../data/tri.obj");

    SceneOperations::openFile("../data/testWireframe3.obj");

    SceneOperations::init_flat_screen();

    ShaderUtils::reshape(window, window_width, window_height);
    ShaderUtils::sendLightsToShaders();

    rng.seed(std::random_device()());

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        manageInput();
        display();
        if(showMenus){
            GUI::draw(window);
        }

        scene_objects[0]->applyTransform();

        glfwSwapBuffers(window);
    }

    SceneOperations::destroyScene();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
