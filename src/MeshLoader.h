//
// Created by bat on 20/11/23.
//

#ifndef OBJECT_VIEWER_MESHLOADER_H
#define OBJECT_VIEWER_MESHLOADER_H

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


#include <iostream>

#include "Mesh.h"
#include "glm/glm.hpp"
#include "Material.h"
#include "stb_image.h"
#include "globals.h"

namespace MeshLoader{

    bool import(const std::string & pFile) {
        // Create an instance of the Importer class
        Assimp::Importer importer;

        // And have it read the given file with some example postprocessing
        // Usually - if speed is not the most important aspect for you - you'll
        // probably to request more postprocessing than we do in this example.
        const aiScene* aiScene = importer.ReadFile(pFile,
                                                  aiProcess_JoinIdenticalVertices |
                                                  aiProcess_Triangulate |
                                                  aiProcess_DropNormals|
                                                  aiProcess_GenSmoothNormals);


        //const aiScene* aiScene = importer.ReadFile(pFile,aiProcessPreset_TargetRealtime_Fast);

        // If the import failed, report it
        if (nullptr == aiScene) {
            std::cout << "Erreur MeshLoader" << std::endl;
            return false;
        }

        int meshNumber = aiScene->mNumMeshes;
        int textureNumber = aiScene->mNumTextures;

        for(int i = 0; i < meshNumber; ++i)
        {
            auto* newMesh = new Mesh();
            Material mat;
            newMesh->material = mat;

            aiMesh* aiMesh = aiScene->mMeshes[i];

            // Now we can access the file's contents.
            unsigned int sizeV = aiMesh->mNumVertices;
            unsigned int sizeT = aiMesh->mNumFaces;

            newMesh->vertices.resize (sizeV);
            newMesh->triangle_indicies.resize (sizeT);
            newMesh->indicies.resize (sizeT*3);
            newMesh->normals.resize(sizeV);

            newMesh->name = aiMesh->mName.C_Str();

            for (unsigned int j = 0; j < sizeV; ++j)
            {
                float x = aiMesh->mVertices[j].x;
                float y = aiMesh->mVertices[j].y;
                float z = aiMesh->mVertices[j].z;
                glm::vec3 pos(x,y,z);
                newMesh->vertices[j] = pos;
            }

            for (unsigned int j = 0; j < sizeV; ++j)
            {
                float x = aiMesh->mNormals[j].x;
                float y = aiMesh->mNormals[j].y;
                float z = aiMesh->mNormals[j].z;
                glm::vec3 normal(x,y,z);
                newMesh->normals[j] = normal;
            }

            for (unsigned int j = 0; j < sizeT; ++j) {
                unsigned int ind1 = aiMesh->mFaces[j].mIndices[0];
                unsigned int ind2 = aiMesh->mFaces[j].mIndices[1];
                unsigned int ind3 = aiMesh->mFaces[j].mIndices[2];

                newMesh->triangle_indicies[j].vertices[0] = ind1;
                newMesh->triangle_indicies[j].vertices[1] = ind2;
                newMesh->triangle_indicies[j].vertices[2] = ind3;

                int offset = j * 3;

                newMesh->indicies[offset] = ind1;
                newMesh->indicies[offset + 1] = ind2;
                newMesh->indicies[offset + 2] = ind3;
            }

            aiAABB aabb = aiMesh->mAABB;

            glm::vec3 bbmin(FLT_MAX,FLT_MAX,FLT_MAX);
            glm::vec3 bbmax(FLT_MIN,FLT_MIN,FLT_MIN);
            for (const glm::vec3& vertex : newMesh->vertices) {
                bbmin[0] = std::min(bbmin[0], vertex[0]);
                bbmin[1] = std::min(bbmin[1], vertex[1]);
                bbmin[2] = std::min(bbmin[2], vertex[2]);

                bbmax[0] = std::max(bbmax[0], vertex[0]);
                bbmax[1] = std::max(bbmax[1], vertex[1]);
                bbmax[2] = std::max(bbmax[2], vertex[2]);
            }
            newMesh->bbmin = bbmin;
            newMesh->bbmax = bbmax;

            // --------------- TEXTURE ---------------- //

            if (aiMesh->HasTextureCoords(0)) {
                // Access texture coordinates


                for(unsigned int texCoord = 0; texCoord < sizeV; ++texCoord)
                {
                    aiVector3D uv = aiMesh->mTextureCoords[0][texCoord];

                    newMesh->uv.emplace_back(uv.x);
                    newMesh->uv.emplace_back(uv.y);
                }

                // Access material
                aiMaterial* material = aiScene->mMaterials[aiMesh->mMaterialIndex];

                // Access the texture count
                unsigned int textureCount = material->GetTextureCount(aiTextureType_DIFFUSE);

                if(textureCount == 0){
                    std::cout << textureCount << "\n";
                }

                // Loop through all textures of the mesh
                for (unsigned int j = 0; j < textureCount; ++j) {
                    aiString texturePath;
                    material->GetTexture(aiTextureType_DIFFUSE, j, &texturePath);

                    const aiTexture* aiTex = aiScene->GetEmbeddedTexture(texturePath.C_Str());

                    Texture texture;

                    // Si la texture est sous un format d'image compréssé
                    // appelle stb_image.h pour décomprésser
                    if(aiTex->mHeight == 0) {

                        int width, height, channels;

                        unsigned char* imageData = stbi_load_from_memory(
                                reinterpret_cast<unsigned char*>(aiTex->pcData),
                                aiTex->mWidth,
                                &width,
                                &height,
                                &channels,
                                STBI_rgb
                        );

                        if(!imageData){
                            std::cerr << "Failed to load embedded compressed texture\n";
                        }

                        texture.height = height;
                        texture.width = width;

                        for(int y = 0; y < height; ++y)
                        {
                            for(int x = 0; x < width; ++x)
                            {
                                int pos = (x*channels) + (y*channels) * width;
                                texture.data.push_back(imageData[pos]);
                                texture.data.push_back(imageData[pos+1]);
                                texture.data.push_back(imageData[pos+2]);
                            }
                        }

                        stbi_image_free(imageData);
                    }
                    else
                    {
                        texture.height = aiTex->mHeight;
                        texture.width = aiTex->mWidth;

                        for(int y = 0; y < aiTex->mHeight; ++y)
                        {
                            for(int x = 0; x < aiTex->mWidth; ++x)
                            {
                                int pos = x + y * aiTex->mWidth;
                                texture.data.push_back(aiTex->pcData[pos].r);
                                texture.data.push_back(aiTex->pcData[pos].g);
                                texture.data.push_back(aiTex->pcData[pos].b);
                            }
                        }
                    }

                    newMesh->material.diffuse_texture = texture;
                    newMesh->material.useTexture = 1;
                }
            }
            else
            {
                // generate 1x1 white texture and uv

                for(auto& a : newMesh->vertices){
                    newMesh->uv.push_back(0);
                    newMesh->uv.push_back(0);
                }

                Texture t = {1,1,std::vector<unsigned char>{255,255,255}};
                newMesh->material.diffuse_texture = t;
                newMesh->material.useTexture = 1;
            }

            // --------------- MATERIAL ---------------- //
            aiMaterial* aiMat = aiScene->mMaterials[aiMesh->mMaterialIndex];

            aiColor3D aiDiff, aiAmb, aiSpec, aiEmissive;
            float aiOpacity, aiShininess, aiIOR, aiShininessStrength, aiRoughness, aiMetallic, aiEmissiveIntensity;

            aiMat->Get(AI_MATKEY_COLOR_AMBIENT, aiAmb);
            aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, aiDiff);
            aiMat->Get(AI_MATKEY_COLOR_SPECULAR, aiSpec);
            aiMat->Get(AI_MATKEY_SHININESS, aiShininess);
            aiMat->Get(AI_MATKEY_METALLIC_FACTOR, aiMetallic);
            aiMat->Get(AI_MATKEY_SHININESS_STRENGTH, aiShininessStrength);
            aiMat->Get(AI_MATKEY_REFRACTI, aiIOR);
            aiMat->Get(AI_MATKEY_ROUGHNESS_FACTOR, aiRoughness);
            aiMat->Get(AI_MATKEY_EMISSIVE_INTENSITY, aiEmissiveIntensity);

            newMesh->material.ambient_material = vec3(aiAmb.r, aiAmb.g, aiAmb.b);
            newMesh->material.diffuse_material = vec3(aiDiff.r, aiDiff.g, aiDiff.b);
            newMesh->material.specular_material = vec3(aiSpec.r, aiSpec.g, aiSpec.b);
            //newMesh->material.IOR = aiIOR;
            //newMesh->material.metallic = aiMetallic;
            //newMesh->material.roughness = aiShininessStrength;
            //newMesh->material.shininess = aiShininess;
            //newMesh->material.emissive_intensity = aiEmissiveIntensity;
            //if(aiEmissiveIntensity > 0)
            //{
                //newMesh->material.isEmissive = 1;
            //}

            // ----------------------------------------- //

            newMesh->openglInit();
            scene_meshes.push_back(newMesh);
        }

        return true;
    }
}

#endif //OBJECT_VIEWER_MESHLOADER_H