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
        if(ImGui::RadioButton("Fast", &render_mode, 0)){
            glUniform1i(render_modeLoc, render_mode);
        }
        ImGui::SameLine();

        if(ImGui::RadioButton("RayTraced", &render_mode, 1)){
            glUniform1i(render_modeLoc, render_mode);
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
        ImGui::InputInt("Denoise window size ",&window_size);
        ImGui::InputFloat("Color distance ", &denoise_bil_colordif_val);
        ImGui::InputFloat("Spatial distance ", &denoise_bil_distance_val);
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
