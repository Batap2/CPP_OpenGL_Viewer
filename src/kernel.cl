#include "src/OpenCL_src/Random.cl"
#include "src/OpenCL_src/Vec3.cl"
#include "src/OpenCL_src/Material.cl"
#include "src/OpenCL_src/Ray.cl"
#include "src/OpenCL_src/HitData.cl"
#include "src/OpenCL_src/Sphere.cl"
#include "src/OpenCL_src/Triangle.cl"
#include "src/OpenCL_src/Plane.cl"
#include "src/OpenCL_src/Bbox.cl"
#include "src/OpenCL_src/Light.cl"
#include "src/OpenCL_src/Scene.cl"
#include "src/OpenCL_src/Camera.cl"

// GLOBAL VARIABLE NAME

//Camera mainCamera;
//Scene *mainScene;
//const int LIGHT_MAX_NUMBER = 32;
//Light lights[LIGHT_MAX_NUMBER];
//int lightNumber = 0;
// Vec3 skyColor;
//const int MESH_MAX_NUMBER = 1000;
//Material materials[MESH_MAX_NUMBER];
//int material_nbr;
//unsigned int GPURandomInt;
//const int TEXTURE_MAX_NUMBER = 300 000 000;
//unsigned char textures[300000000];


//To update the camera
__kernel void updateCamera(int max_x, int max_y,__global float* cameraData)
{
    int i = get_global_id(0);
    int j = get_global_id(1);

    if(i==0 && j == 0)
    {
        mainCamera.cameraPos = (Vec3){cameraData[0], cameraData[1], cameraData[2]};
        mainCamera.cameraDirection = (Vec3){cameraData[3+0], cameraData[3+1], cameraData[3+2]};
        mainCamera.cameraRight = (Vec3){cameraData[6+0], cameraData[6+1], cameraData[6+2]};
        mainCamera.cameraUp = cross(mainCamera.cameraDirection, mainCamera.cameraRight);
        mainCamera.FOV = 60.;
        mainCamera.aspectRatio = (float)max_x / (float)max_y;
        mainCamera.tanFOV = tan(0.5 * radians(mainCamera.FOV));
    }
}

//To update the skyColor
__kernel void updateSkyColor(int max_x, int max_y,__global float* SkyColor)
{
    int i = get_global_id(0);
    int j = get_global_id(1);

    if(i==0 && j == 1)
    {
        skyColor = (Vec3){SkyColor[0],SkyColor[1],SkyColor[2]};
    }
}

//To update materials
__kernel void updateMaterials(int max_x, int max_y, int mesh_nbr, __global float* materialsData) {
    int i = get_global_id(0);
    int j = get_global_id(1);

    if (i == 0 && j == 2) {
        material_nbr = mesh_nbr;
        for(int mesh_id =0; mesh_id < mesh_nbr; mesh_id++)
        {
            unsigned int mat_id = mesh_id*16;
            materials[mesh_id].ambiant_color = (Vec3){materialsData[mat_id],materialsData[mat_id+1],materialsData[mat_id+2]};
            materials[mesh_id].diffuse_color = (Vec3){materialsData[mat_id+3],materialsData[mat_id+4],materialsData[mat_id+5]};
            materials[mesh_id].specular_color = (Vec3){materialsData[mat_id+6],materialsData[mat_id+7],materialsData[mat_id+8]};
            materials[mesh_id].shininess = materialsData[mat_id+9];
            materials[mesh_id].metallic = materialsData[mat_id+10];
            materials[mesh_id].roughness = materialsData[mat_id+11];
            materials[mesh_id].ao = materialsData[mat_id+12];
            materials[mesh_id].emissiveIntensity = materialsData[mat_id+13];
                        materials[mesh_id].isTransparent = materialsData[mat_id+14];
                        materials[mesh_id].IOR = materialsData[mat_id+15];

        }
    }
}




//To update lights
__kernel void updateLights(int max_x, int max_y, int light_nbr, __global float* lightsData) {
    int i = get_global_id(0);
    int j = get_global_id(1);

    if (i == 0 && j == 3) {
        lightNumber = light_nbr;
            for(int light_id =0; light_id< light_nbr; light_id++) // a décommenter, c'est juste trop abusé l'intensité lumineuse de deux lights
            {
                unsigned int l_id = light_id*11;
                lights[light_id].color = (Vec3){lightsData[l_id+8],lightsData[l_id+9],lightsData[l_id+10]};
                lights[light_id].pos= (Vec3){lightsData[l_id+2],lightsData[l_id+3],lightsData[l_id+4]};
                lights[light_id].intensity=lightsData[l_id+6];
            }
    }
}

//To load all data requiered to initialize the kernel
__kernel void loading(int max_x, int max_y,__global float* cameraData,__global float* SkyColor, int mesh_nbr,__global float* materialsData,__global float* vertices,__global unsigned int* indices,__global unsigned int* split_meshes,__global unsigned int* split_meshes_tri,__global float * lightsData,int light_nbr, __global float * bboxs, __global unsigned char * texturesData, __global unsigned int * split_textures, int texturesize, __global float * uvsData, __global unsigned int * split_uvs){
    int i = get_global_id(0);
    int j = get_global_id(1);
    material_nbr = mesh_nbr;
    scene_size = mesh_nbr;

    //Loading camera
    if( i==0 && j == 0)
    {
        mainCamera.cameraPos = (Vec3){cameraData[0], cameraData[1], cameraData[2]};
        mainCamera.cameraDirection = (Vec3){cameraData[3+0], cameraData[3+1], cameraData[3+2]};
        mainCamera.cameraRight = (Vec3){cameraData[6+0], cameraData[6+1], cameraData[6+2]};
        mainCamera.cameraUp = cross(mainCamera.cameraDirection, mainCamera.cameraRight);
        mainCamera.FOV = 60.;
        mainCamera.aspectRatio = (float)max_x / (float)max_y;
        mainCamera.tanFOV = tan(0.5 * radians(mainCamera.FOV));
    }

    //Loading SkyColor
    if(i==1 && j ==0)
    {
        skyColor = (Vec3){SkyColor[0],SkyColor[1],SkyColor[2]};
    }


    //Loading Material
    if(i==2 && j==0)
    {



                for(int tex_id =0; tex_id < texturesize; tex_id++)
                {
                    textures[tex_id] = texturesData[tex_id];
                }

        for(int mesh_id =0; mesh_id < mesh_nbr; mesh_id++)
        {

            unsigned int mat_id = mesh_id*16;
            materials[mesh_id].ambiant_color = (Vec3){materialsData[mat_id],materialsData[mat_id+1],materialsData[mat_id+2]};
            materials[mesh_id].diffuse_color = (Vec3){materialsData[mat_id+3],materialsData[mat_id+4],materialsData[mat_id+5]};
            materials[mesh_id].specular_color = (Vec3){materialsData[mat_id+6],materialsData[mat_id+7],materialsData[mat_id+8]};
            materials[mesh_id].shininess = materialsData[mat_id+9];
            materials[mesh_id].metallic = materialsData[mat_id+10];
            materials[mesh_id].roughness = materialsData[mat_id+11];
            materials[mesh_id].ao = materialsData[mat_id+12];
            materials[mesh_id].emissiveIntensity = materialsData[mat_id+13];
            materials[mesh_id].isTransparent = materialsData[mat_id+14];
            materials[mesh_id].IOR = materialsData[mat_id+15];

            unsigned int tex_id = mesh_id* 3;
            materials[mesh_id].texture.width = split_textures[tex_id];
            materials[mesh_id].texture.height = split_textures[tex_id+1];
            materials[mesh_id].texture.offset = split_textures[tex_id+2];

            if( !(bool)materials[mesh_id].texture.width && !(bool)materials[mesh_id].texture.height)
            {
                materials[mesh_id].useTexture = false;
            }
            else{
                materials[mesh_id].useTexture = true;
                //materials[mesh_id].diffuse_color = (Vec3){(int)texturesData[materials[mesh_id].texture.offset]/255.f,(int)texturesData[materials[mesh_id].texture.offset+1]/255.f,(int)texturesData[materials[mesh_id].texture.offset+2]/255.f};

            }
            // TODO : rajouter cela
            //materials[mesh_id].isTransparent = 0.0f;
            //materials[mesh_id].IOR = 1.4f;
        }
    }




    //Loading Scene - mostly Triangles
    if(i==2 && j==0)
    {

		mainScene2.numSpheres = 0;
        mainScene2.numPlanes = 0;
        mainScene2.numTriangles = 0;
        mainScene2.numBboxes = 0;


        // DEBUG ---------------------------------------------------------------
        //Adding spheres and his matereial

        // --------------------scene : big_plane

        // addSphere((Vec3){0.0f,0.35f,0.0f}, 0.3,3);

        // addSphere((Vec3){-0.5f,1.35f,0.0f}, 0.3,1);

        // addSphere((Vec3){-1.0f,0.35f,0.0f}, 0.3,2);


        // materials[0].roughness = 0.8f;
        // materials[0].diffuse_color = (Vec3){0.4f,0.4f,0.4f};

        // materials[1].roughness = 0.6f;
        // materials[1].diffuse_color = (Vec3){1.0f,1.0f,1.0f};
        // materials[1].emissiveIntensity = 10.0f;

        // materials[2].roughness = 1.0f;
        // materials[2].diffuse_color = (Vec3){1.0f,0.0f,0.0f};

        // materials[3].roughness = 0.0f;
        // materials[3].isTransparent = 1.0f;


        // --------------------scene : cornel

        // int mat_created_id = createMaterial(materials[2].ambiant_color,(Vec3){1.0f,1.0f,1.0f},materials[2].specular_color,materials[2].shininess,0.0f,0.0f,materials[2].ao);
        // addSphere((Vec3){0.0f,-0.6f,1.5f}, 0.2,mat_created_id);

        // int mat_created_id2 = createMaterial(materials[2].ambiant_color,(Vec3){1.0f,1.0f,1.0f},materials[2].specular_color,materials[2].shininess,0.0f,1.0f,materials[2].ao);
        // addSphere((Vec3){-0.5f,-0.6f,1.5f}, 0.2,mat_created_id2);

        // int mat_created_id3 = createMaterial(materials[2].ambiant_color,(Vec3){1.0f,1.0f,1.0f},materials[2].specular_color,materials[2].shininess,0.0f,0.0f,materials[2].ao);
        // addSphere((Vec3){0.5f,-0.6f,1.5f}, 0.2,mat_created_id3);

        // materials[mat_created_id3].isTransparent = 1.0f;

        // materials[6].emissiveIntensity = 2.0f;

        // --------------------scene : plan_cyl_tex

        //materials[2].diffuse_color = (Vec3){1.0f,0.0f,0.0f};
        // materials[2].roughness = 0.0f;
        // materials[2].isTransparent = 1.0f;

        // --------------------scene : sky

        // int mat_created_id = createMaterial(materials[2].ambiant_color,(Vec3){1.0f,1.0f,1.0f},materials[2].specular_color,materials[2].shininess,0.0f,0.002f,materials[2].ao);
        // addSphere((Vec3){0.0f,0.2f,0.0f}, 0.2,mat_created_id);

        // int mat_created_id2 = createMaterial(materials[2].ambiant_color,(Vec3){1.0f,0.0f,0.0f},materials[2].specular_color,0.0f,0.0f,1.0f,materials[2].ao);
        // addSphere((Vec3){-0.5f,0.2f,0.0f}, 0.2,mat_created_id2);

        // int mat_created_id3 = createMaterial(materials[2].ambiant_color,(Vec3){1.0f,1.0f,1.0f},materials[2].specular_color,materials[2].shininess,0.0f,0.0f,materials[2].ao);
        // addSphere((Vec3){0.5f,0.2f,0.0f}, 0.2,mat_created_id3);

        // materials[mat_created_id3].isTransparent = 1.0f;

        // DEBUG ---------------------------------------------------------------



        unsigned int offset_vertex = 0;
        unsigned int offset_index = 0;
        unsigned int offset_tri = 0;
        for(int mesh_id = 0; mesh_id < mesh_nbr; mesh_id++)
        {
            unsigned int vertex_nbr = split_meshes[mesh_id];
            unsigned int tri_nbr = split_meshes_tri[mesh_id];
            unsigned int index_nbr = tri_nbr *3;
            unsigned int uv_id = mesh_id * 2;
            unsigned int uv_offset = split_uvs[uv_id];
            unsigned int uv_nbr = split_uvs[uv_id+1];



            for(int tri = 0; tri < tri_nbr; tri++)
            {
                int current_id = offset_index + tri*3 ;
                int id0 = current_id;
                int id1 = current_id + 1;
                int id2 = current_id + 2;

                Vec3 s0 = (Vec3){vertices[(offset_vertex+indices[id0])*3+0],vertices[(offset_vertex+indices[id0])*3+1],vertices[(offset_vertex+indices[id0])*3+2]};
                float2 uv0;
                uv0.x = uvsData[uv_offset + indices[id0] * 2];
                uv0.y = uvsData[uv_offset + indices[id0] * 2 + 1];
                Vec3 s1 = (Vec3){vertices[(offset_vertex+indices[id1])*3+0],vertices[(offset_vertex+indices[id1])*3+1],vertices[(offset_vertex+indices[id1])*3+2]};
                float2 uv1;
                uv1.x = uvsData[uv_offset + indices[id1] * 2];
                uv1.y = uvsData[uv_offset + indices[id1] * 2 + 1];
                Vec3 s2 = (Vec3){vertices[(offset_vertex+indices[id2])*3+0],vertices[(offset_vertex+indices[id2])*3+1],vertices[(offset_vertex+indices[id2])*3+2]};
                float2 uv2;
                uv2.x = uvsData[uv_offset + indices[id2] * 2];
                uv2.y = uvsData[uv_offset + indices[id2] * 2 + 1];

                int current_tri_id =tri + offset_index/3;
                mainScene2.triangles[current_tri_id].vertex1 = s0;
                mainScene2.triangles[current_tri_id].vertex2 = s1;
                mainScene2.triangles[current_tri_id].vertex3 = s2;
                mainScene2.triangles[current_tri_id].uv0 = uv0;
                mainScene2.triangles[current_tri_id].uv1 = uv1;
                mainScene2.triangles[current_tri_id].uv2 = uv2;
                mainScene2.triangles[current_tri_id].mat = mesh_id;
                mainScene2.numTriangles++;
            }
            offset_index += index_nbr;
            offset_vertex += vertex_nbr;
            offset_tri += tri_nbr;
        }

                    int bboxid = 0;
                    Bbox current_bbox;
                    current_bbox.bbmin = (Vec3){bboxs[bboxid],bboxs[bboxid+1],bboxs[bboxid+2]};
                    current_bbox.bbmax = (Vec3){bboxs[bboxid+3],bboxs[bboxid+4],bboxs[bboxid+5]};
                    current_bbox.offset_triangle = 0;
                    current_bbox.triangle_nbr = offset_tri;
                    current_bbox.trianglesOfBox = &mainScene2.triangles;
                    mainScene2.bboxes[0] = current_bbox;
                    mainScene2.numBboxes++;

        generateKdTree(&mainScene2.bboxes[0], 3 , 0);

    }
    if(i==3 && j==0)
    {
        for(int light_id =0; light_id< light_nbr; light_id++) // a décommenter, c'est juste trop abusé l'intensité lumineuse de deux lights
        {
            unsigned int l_id = light_id*11;
            Vec3 color = (Vec3){lightsData[l_id+8],lightsData[l_id+9],lightsData[l_id+10]};
            Vec3 pos= (Vec3){lightsData[l_id+2],lightsData[l_id+3],lightsData[l_id+4]};
            float intensity = lightsData[l_id+6];
            addLight(pos,color,intensity);
        }
    }

}


//Fonction principale du kernel -> Rendu par raytracing 'une image de la scène
__kernel void render(__global float* fb, int max_x, int max_y) //,__global unsigned char * texturesData, int size
{
	int i = get_global_id(0);
	int j = get_global_id(1);

    unsigned int randomSeedInit = GPURandomInt;
	randomSeedInit = randomSeedInit * i*i*j*j;
	randomSeed = &randomSeedInit;

    if ((i < max_x) && (j < max_y)) {
    	/*if(i == 0 && j == 0)
            {
                for(int tex_id = 0; tex_id < size; tex_id++)
                {
                    texturesData[tex_id] = textures[tex_id];
                }
            }*/
        int pixel_index = j * max_x * 3 + i * 3;

		// Calcul des coordonnées du rayon
		Ray ray = getRayfromCamera(mainCamera,i,j,max_x,max_y);

		//addLight((Vec3){-0.75f,1.0f,1.2f}, (Vec3){1.0f,0.8f,0.55f}, 0.1f);
		//addLight((Vec3){0.75f,1.0f,1.2f}, (Vec3){0.85f,0.95f,1.0f}, 0.1f);

		
		int bounce = 15;

        // anti aliasing
        ray.direction =  randomizeInHemiSphere_fast(ray.direction, 0.0002f);
	
		Vec3 out_color = computeColor(&ray, mainCamera.cameraPos, bounce);

		fb[pixel_index + 0] = out_color.x;
		fb[pixel_index + 1] = out_color.y;
		fb[pixel_index + 2] = out_color.z;
    }
}

__kernel void denoise_avg(__global float* fb, int window_size, int max_x, int max_y)
{
    int i = get_global_id(0);
    int j = get_global_id(1);

    if ((i < max_x) && (j < max_y)) {
        int pixel_index = j * max_x * 3 + i * 3;

        // Initialisation des variables pour la moyenne
        float sum_r = 0.0f;
        float sum_g = 0.0f;
        float sum_b = 0.0f;

        // Taille de la fenêtre de moyenne
        int window_size = 1;
        // Calcul de la moyenne dans la fenêtre
        for (int di = -window_size; di <= window_size; di++) {
            for (int dj = -window_size; dj <= window_size; dj++) {
                int ni = i + di;
                int nj = j + dj;

                if (ni >= 0 && ni < max_x && nj >= 0 && nj < max_y) {
                    int neighbor_index = nj * max_x * 3 + ni * 3;
                    sum_r += fb[neighbor_index + 0];
                    sum_g += fb[neighbor_index + 1];
                    sum_b += fb[neighbor_index + 2];
                }
            }
        }

        // Calcul de la moyenne
        int num_pixels_in_window = (2 * window_size + 1) * (2 * window_size + 1);
        fb[pixel_index + 0] = sum_r / num_pixels_in_window;
        fb[pixel_index + 1] = sum_g / num_pixels_in_window;
        fb[pixel_index + 2] = sum_b / num_pixels_in_window;
    }
}


// Fonction de noyau pour le filtre bilatéral
__kernel void denoise_bil(__global float* fb, int max_x, int max_y, int window_size, float spatial_sigma, float intensity_sigma)
{
        int i = get_global_id(0);
        int j = get_global_id(1);

        if ((i < max_x) && (j < max_y)) {
            int pixel_index = j * max_x * 3 + i * 3;


        // Pixel d'origine
        float center_r = fb[pixel_index + 0];
        float center_g = fb[pixel_index + 1];
        float center_b = fb[pixel_index + 2];

            // Initialisation des variables pour la moyenne
            float sum_r = 0.0f;
            float sum_g = 0.0f;
            float sum_b = 0.0f;
            float weight_sum = 0.0f;

            // Calcul de la moyenne dans la fenêtre
            for (int di = -window_size; di <= window_size; di++) {
                for (int dj = -window_size; dj <= window_size; dj++) {
                    int ni = i + di;
                    int nj = j + dj;



                    if (ni >= 0 && ni < max_x && nj >= 0 && nj < max_y) {
                        int neighbor_index = nj * max_x * 3 + ni * 3;
                    // Pixel voisin
                    float neighbor_r = fb[neighbor_index + 0];
                    float neighbor_g = fb[neighbor_index + 1];
                    float neighbor_b = fb[neighbor_index + 2];

                    // Calcul des poids spatial et d'intensité
                    float spatial_weight = exp(-(di * di + dj * dj) / (2.0f * spatial_sigma * spatial_sigma));
                    float intensity_weight = exp(-((center_r - neighbor_r) * (center_r - neighbor_r) + (center_g - neighbor_g) * (center_g - neighbor_g) + (center_b - neighbor_b) * (center_b - neighbor_b)) /(2.0f * intensity_sigma * intensity_sigma));

                    float weight = spatial_weight * intensity_weight;

                    // Accumulation pondérée
                    sum_r += neighbor_r * weight;
                    sum_g += neighbor_g * weight;
                    sum_b += neighbor_b * weight;

                    // Accumulation des poids
                    weight_sum += weight;


                    }
                }
            }


        // Calcul de la moyenne pondérée
        fb[pixel_index + 0] = sum_r / weight_sum;
        fb[pixel_index + 1] = sum_g / weight_sum;
        fb[pixel_index + 2] = sum_b / weight_sum;
        }
}

__kernel void getRandomIntInGPU(unsigned int CPURandomInt)
{
    GPURandomInt = CPURandomInt;
}