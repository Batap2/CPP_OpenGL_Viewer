//
// Created by bat on 20/11/23.
//

#ifndef OBJECT_VIEWER_GUI_H
#define OBJECT_VIEWER_GUI_H

#include <cmath>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui-filebrowser/imfilebrowser.h"
#include "ImGuizmo/ImGuizmo.h"


#include "ShaderUtils.h"
#include "SceneOperations.h"

#include "globals.h"

// FLAGS : https://pixtur.github.io/mkdocs-for-imgui/site/api-imgui/Flags---Enumerations/#ImGuiWindowFlags_NoMove
// GIZMO : https://github.com/CedricGuillemet/ImGuizmo
// FILE DIALOG : https://github.com/gallickgunner/ImGui-Addons

namespace GUI{

    void init(){
        fileDialog.SetTitle("Open File");
        fileDialog.SetTypeFilters({ ".obj", ".fbx", ".glb", ".gltf" });
    }

    void displayMainToolbar(GLFWwindow* window){

        ImGui::SetNextWindowPos({0, 0});

        float panelW = std::fmax(static_cast<float>(window_width)/4, 400.0f);
        ImGui::SetNextWindowSize({panelW, static_cast<float>(window_height)});
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImColor(0,0,0,100).Value);

        auto  flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar;

        ////////////////////////////////////////////////////////////////////////////////////////////////

        ImGui::Begin("Scene Configuration", &imguitest1, flags );
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open..", "Ctrl+O"))
                {
                    fileDialog.Open();
                }

                if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
                if (ImGui::MenuItem("Close", "Ctrl+W"))  {}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////

        ImGui::Separator();
        ImGui::Separator();
        ImGui::TextColored({0.0f,1.0f,1.0f,1.0f}, "Render Mode");
        ImGui::SameLine();
        ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);
        if(ImGui::RadioButton("Smooth", &render_mode, 0)){
            //glUniform1i(render_modeLoc, render_mode);
        }
        ImGui::SameLine();

        if(ImGui::RadioButton("Flat", &render_mode, 1)){
            //glUniform1i(render_modeLoc, render_mode);
        }

        ImGui::Checkbox("Display normals", &displayNormals);

        if(ImGui::DragFloat("Normals length", &normalDisplayLength, 0.001, 0, 9999)){

            glUseProgram(shaderProgram_NormalDisplay);
            glUniform1f(normalDisplayLengthLoc, normalDisplayLength);
        }

        ImGui::SliderInt("Wireframe Mode", &wireframeMode, 0, 2);

        if(ImGui::DragFloat("Wireframe width", &wireframeWidth, 0.00001, 0, 0.1)){
            glUseProgram(shaderProgram_WireframeDisplay);
            glUniform1f(wireframeWidthLocWS, wireframeWidth);
            glUseProgram(shaderProgram_frameBufferWireframe);
            glUniform1f(wireframeWidthLocFBS, wireframeWidth);
            glUseProgram(shaderProgram_BarycentricWireframe);
            glUniform1f(wireframeWidthLocBWS, wireframeWidth);
        }


        if(ImGui::ColorEdit4("Wireframe Color", &wireFrameColor[0]), colorEditFlag)
        {
            if(oldWireFrameColor != wireFrameColor)
            {
                glUseProgram(shaderProgram_WireframeDisplay);
                glUniform4fv(wireframeColorLocWS, 1, &wireFrameColor[0]);
                glUseProgram(shaderProgram_frameBufferWireframe);
                glUniform4fv(wireframeColorLocFBS, 1, &wireFrameColor[0]);
                glUseProgram(shaderProgram_BarycentricWireframe);
                glUniform4fv(wireframeColorLocBWS, 1, &wireFrameColor[0]);

                oldWireFrameColor = wireFrameColor;
            }
        }

        ImGui::Separator();ImGui::Separator();
        ImGui::Text(" ");ImGui::Text(" ");

        ////////////////////////////////////////////////////////////////////////////////////////////////

        ImGui::Separator();ImGui::Separator();
        ImGui::TextColored({0.0f,1.0f,1.0f,1.0f}, "Camera & World");
        ImGui::Separator();

        if(ImGui::SliderFloat("Field of view", &fovy, 0.0f, 180.0f)) {
            ShaderUtils::reshape(window, window_width, window_height);
        }

        ImGui::SliderFloat("Frustum near plane", &zNear, 0.0f, 15.0f);
        ImGui::SliderFloat("Frustum far plane", &zFar, 0.0f, 150.0f);
        ImGui::Text(" ");
        ImGui::InputFloat3("Camera position ", &mainCamera.cameraPos[0]);
        ImGui::InputFloat3("Camera orientation ", &mainCamera.cameraDirection[0]);
        ImGui::Text(" ");
        if(ImGui::ColorEdit3("Sky Color", &skyColor[0]))
        {
        }
        ImGui::Separator();ImGui::Separator();

        ImGui::Text(" ");ImGui::Text(" ");

        ImGui::Separator();ImGui::Separator();
        ImGui::TextColored({0.0f,1.0f,1.0f,1.0f}, "Scene Objects");
        ImGui::Separator();


        ImGui::BeginChild("1", ImVec2(0,100));


        if (ImGui::BeginPopupContextItem("objProperties"))
        {
            if(ImGui::ColorEdit3("Color", &scene_meshes[selected_object]->material.diffuse_material[0]), colorEditFlag)
            {
                if(old_mesh_color != scene_meshes[selected_object]->material.diffuse_material)
                {
                    scene_meshes[selected_object]->send_material_to_shaders();
                    old_mesh_color = scene_meshes[selected_object]->material.diffuse_material;
                    render_number = 0;
                }
            }
            if(ImGui::DragFloat("Metallic", &scene_meshes[selected_object]->material.metallic, 0.01, 0, 1))
            {
                scene_meshes[selected_object]->send_material_to_shaders();
                render_number = 0;
            }
            if(ImGui::DragFloat("Roughness", &scene_meshes[selected_object]->material.roughness, 0.01, 0, 1))
            {
                scene_meshes[selected_object]->send_material_to_shaders();
                render_number = 0;
            }
            if(ImGui::DragFloat("Emissive Intensity", &scene_meshes[selected_object]->material.emissive_intensity, 0.01, 0, 50))
            {
                scene_meshes[selected_object]->send_material_to_shaders();
                render_number = 0;
            }


            if(ImGui::DragFloat3("Position", &scene_objects[selected_object]->position[0]), 0.01f){
                //TODO : fix ça, j'ai la flemme pour l'instant
                scene_objects[selected_object]->updatePosition();
                scene_objects[selected_object]->applyTransform();
            }

            if(scene_meshes[selected_object]->material.isEmissive){
                emissiveClick = true;
            } else {
                emissiveClick = false;
            }

            if(scene_meshes[selected_object]->material.isTransparent){
                transparentClick = true;
            } else {
                transparentClick = false;
            }

            if(scene_meshes[selected_object]->visible){
                visibleClick = true;
            } else {
                visibleClick = false;
            }

            if(ImGui::Checkbox("Emissive", &emissiveClick)){
                if(emissiveClick != scene_meshes[selected_object]->material.isEmissive){
                    if(emissiveClick){
                        scene_meshes[selected_object]->material.isEmissive = 1.0f;
                    } else {
                        scene_meshes[selected_object]->material.isEmissive = 0.0f;
                    }
                    scene_meshes[selected_object]->send_material_to_shaders();
                    render_number = 0;
                }
            }
            if(ImGui::Checkbox("Transparent", &transparentClick)){
                if(transparentClick != scene_meshes[selected_object]->material.isTransparent){
                    if(transparentClick){
                        scene_meshes[selected_object]->material.isTransparent = 1.0f;
                    } else {
                        scene_meshes[selected_object]->material.isTransparent = 0.0f;
                    }
                    scene_meshes[selected_object]->send_material_to_shaders();
                    render_number = 0;
                }
            }

            if(ImGui::Checkbox("Visible", &visibleClick)){
                if(visibleClick != scene_meshes[selected_object]->visible){
                    if(visibleClick){
                        scene_meshes[selected_object]->visible = true;
                    } else {
                        scene_meshes[selected_object]->visible = false;
                    }
                    scene_meshes[selected_object]->send_material_to_shaders();
                    render_number = 0;
                }
            }

            if(ImGui::Button("Delete"))
            {
                SceneOperations::removeMesh(selected_object);
                render_number = 0;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();

        }

        for(int i = 0; i < scene_meshes.size(); ++i)
        {

            if(ImGui::Button(scene_meshes[i]->name.c_str()))
            {
                selected_object = i;
                old_mesh_color = scene_meshes[selected_object]->material.diffuse_material;
                ImGui::OpenPopup("objProperties");
            }
        }
        ImGui::EndChild();

        ImGui::Separator();ImGui::Separator();

        ImGui::Text(" ");ImGui::Text(" ");

        ////////////////////////////////////////////////////////////////////////////////////////////////

        ImGui::Separator();ImGui::Separator();

        ImGui::TextColored({0.0f,1.0f,1.0f,1.0f}, "Lights");
        ImGui::SameLine();
        ImGui::Dummy(ImVec2(10,0));
        ImGui::SameLine();
        if(ImGui::Button("Add Light"))
        {
            SceneOperations::addLight();
            render_number = 0;
        }

        ImGui::Separator();

        ImGui::BeginChild("2", ImVec2(0,100));

            if (ImGui::BeginPopupContextItem("lightsProperties"))
            {
                if(ImGui::DragFloat3("Position", &scene_lights[selected_light]->pos[0], 0.01))
                {
                    ShaderUtils::sendLightsToShaders();
                    render_number = 0;
                }
                if(ImGui::ColorEdit3("Color", &scene_lights[selected_light]->color[0]), colorEditFlag)
                {
                    if(old_light_color != scene_lights[selected_light]->color){
                        ShaderUtils::sendLightsToShaders();
                        old_light_color = scene_lights[selected_light]->color;
                        render_number = 0;
                    }
                }
                if(ImGui::DragFloat("Intensity", &scene_lights[selected_light]->intensity, 0.01))
                {
                    ShaderUtils::sendLightsToShaders();
                    render_number = 0;
                }

                if(ImGui::Button("Delete"))
                {
                    SceneOperations::removeLight(selected_light);
                    ImGui::CloseCurrentPopup();
                    render_number = 0;
                }

                ImGui::EndPopup();

            }

            for(int i = 0; i < scene_lights.size(); ++i)
            {
                std::string name = "Light " + std::to_string(i);
                if(ImGui::Button(name.c_str()))
                {
                    selected_light = i;
                    old_light_color = scene_lights[selected_light]->color;
                    ImGui::OpenPopup("lightsProperties");
                }
            }
        ImGui::EndChild();

        ImGui::Separator();ImGui::Separator();

        ImGui::PopStyleColor();
        ImGui::End();
    }

    void draw(GLFWwindow* window){
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();

        //TODO : enlever cette merde
//        auto Frustum = [&](float left, float right, float bottom, float top, float znear, float zfar, float* m16)
//        {
//            float temp, temp2, temp3, temp4;
//            temp = 2.0f * znear;
//            temp2 = right - left;
//            temp3 = top - bottom;
//            temp4 = zfar - znear;
//            m16[0] = temp / temp2;
//            m16[1] = 0.0;
//            m16[2] = 0.0;
//            m16[3] = 0.0;
//            m16[4] = 0.0;
//            m16[5] = temp / temp3;
//            m16[6] = 0.0;
//            m16[7] = 0.0;
//            m16[8] = (right + left) / temp2;
//            m16[9] = (top + bottom) / temp3;
//            m16[10] = (-zfar - znear) / temp4;
//            m16[11] = -1.0f;
//            m16[12] = 0.0;
//            m16[13] = 0.0;
//            m16[14] = (-temp * zfar) / temp4;
//            m16[15] = 0.0;
//        };
//
//        auto Perspective = [&](float fovyInDegrees, float aspectRatio, float znear, float zfar, float* m16)
//        {
//            float ymax, xmax;
//            ymax = znear * tanf(fovyInDegrees * 3.141592f / 180.0f);
//            xmax = ymax * aspectRatio;
//            Frustum(-xmax, xmax, -ymax, ymax, znear, zfar, m16);
//        };
//
//        float aprojection[16];
//
//        float aobj[16] =
//                {
//                        1,0,0,0,
//                        0,1,0,0,
//                        0,0,1,0,
//                        0,0,0,1
//                };
//
//        float aview[16] =
//                {
//                1,0,0,0,
//                0,1,0,0,
//                0,0,1,0,
//                0,0,0,1
//                };
//
//        ImGuiIO& io = ImGui::GetIO();
//        Perspective(27, io.DisplaySize.x / io.DisplaySize.y, 0.1f, 100.f, aprojection);
//
//        ImGuizmo::Enable(true);
//        ImGuizmo::DrawCubes(aview, aprojection, aobj, 1);


        displayMainToolbar(window);
        fileDialog.Display();

        if(fileDialog.HasSelected())
        {
            SceneOperations::openFile(fileDialog.GetSelected().string());
            fileDialog.ClearSelected();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
};

#endif //OBJECT_VIEWER_GUI_H
