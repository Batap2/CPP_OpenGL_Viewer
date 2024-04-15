//
// Created by bat on 22/11/23.
//

#ifndef OBJECT_VIEWER_SCENEOPERATIONS_H
#define OBJECT_VIEWER_SCENEOPERATIONS_H

#include "globals.h"
#include "MeshLoader.h"


namespace SceneOperations
{

    void initSceneLights()
    {
        auto* l1 = new Light();
        auto* l2 = new Light();

        l1->pos = glm::vec3(-1000,1000,1000);
        l2->pos = glm::vec3(0.75,1,1.2);

        l1->color = glm::vec3((float)255/255,(float)190/255,(float)136/255);
        l2->color = glm::vec3((float)198/255,(float)216/255,(float)255/255);

        l1->intensity = 4;
        l2->intensity = 1;

        scene_lights.push_back(l1);
        scene_lights.push_back(l2);
    }

    void init_flat_screen()
    {
        flat_screen.vertices.push_back(vec3(-1, 1, 0));
        flat_screen.vertices.push_back(vec3(-1, -1, 0));
        flat_screen.vertices.push_back(vec3(1, -1, 0));
        flat_screen.vertices.push_back(vec3(1, 1, 0));

        flat_screen.normals.push_back(vec3(0));
        flat_screen.normals.push_back(vec3(0));
        flat_screen.normals.push_back(vec3(0));
        flat_screen.normals.push_back(vec3(0));

        flat_screen.uv.push_back(0);
        flat_screen.uv.push_back(1);

        flat_screen.uv.push_back(0);
        flat_screen.uv.push_back(0);

        flat_screen.uv.push_back(1);
        flat_screen.uv.push_back(0);

        flat_screen.uv.push_back(1);
        flat_screen.uv.push_back(1);

        flat_screen.material.diffuse_texture.height = window_height;
        flat_screen.material.diffuse_texture.width = window_width;

        for(int x = 0; x < window_width; ++x)
        {
            for(int y = 0; y < window_height; ++y)
            {
                flat_screen.material.diffuse_texture.data.push_back(255);
                flat_screen.material.diffuse_texture.data.push_back(255);
                flat_screen.material.diffuse_texture.data.push_back(255);
            }
        }


        flat_screen.indicies.push_back(0);
        flat_screen.indicies.push_back(1);
        flat_screen.indicies.push_back(2);
        flat_screen.indicies.push_back(2);
        flat_screen.indicies.push_back(3);
        flat_screen.indicies.push_back(0);

        flat_screen.openglInit();
    }

    void destroyScene(){
        for (auto& lightPtr : scene_lights) {
            delete lightPtr;
        }

        for (auto& meshPtr : scene_meshes) {
            delete meshPtr;
        }
    }

    int addLight()
    {
        if(scene_lights.size() >= lightsMaxNumber){
            std::cout << "max scene_lights number reached\n";
            return -1;
        }

        auto *l = new Light();

        scene_lights.push_back(l);

        ShaderUtils::sendLightsToShaders();

        return 1;
    }

    int removeLight(int index)
    {
        if(scene_lights.size() == 0){
            std::cout << "there is no light\n";
            return -1;
        }

        if(scene_lights.size() < index){
            std::cout << "out of range\n";
            return -1;
        }
        scene_lights.erase(scene_lights.begin()+index);

        ShaderUtils::sendLightsToShaders();

        return 1;
    }

    int removeMesh(int index)
    {
        if(scene_meshes.size() == 0){
            std::cout << "there is no mesh\n";
            return -1;
        }

        if(scene_meshes.size() < index){
            std::cout << "out of range\n";
            return -1;
        }

        scene_meshes.erase(scene_meshes.begin()+index);

        return 1;
    }

    void openFile(std::string path)
    {
        std::cout << path << "\n";
        MeshLoader::import(path);

        for(Mesh* m : scene_meshes)
        {
            auto* newObj = new Object3D();
            newObj->mesh = m;

            scene_objects.push_back(newObj);
        }
    }
}

#endif //OBJECT_VIEWER_SCENEOPERATIONS_H
