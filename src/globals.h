#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include "Object3D.h"
#include "Light.h"
#include "transform.h"
#include "camera.h"
#include "imgui-filebrowser/imfilebrowser.h"
#include <random>


// -------------- MENU APPEARANCE -------------- //
inline float screenSeparation1 = 4;

// -------------- KEYS -------------- //
inline bool rclickHolded, upHolded, downHolded, leftHolded, rightHolded, aHolded, eHolded;


inline int amount;
inline vec3 eye;
inline vec3 up;


inline vec3 eyeinit(0.0, 0.2, 4.0);
inline vec3 upinit(0.0, 1.0, 0.0);
inline vec3 center(0.0, 0.0, 0.0);
inline int mouse_mode    = 1;
inline int keyboard_mode = 0;
inline int window_width  = 1600;
inline int window_height =900;
inline int amountinit    = 5;
inline float fovy        = 60.0f;
inline float zFar        = 99.0f;
inline float zNear       = 0.1f;
inline bool firstMouse = true;
inline float yaw   = -90.0f;
inline float pitch =  0.0f;
inline float lastX =  window_width / 2.0;
inline float lastY =  window_height / 2.0;

inline mat4 projection, modelview, model, view;

inline std::mt19937 rng;

// -------------- SHADER & UNIFORM LOCATIONS -------------- //
inline GLuint vertexshader, geometryShader, fragmentshader, shaderprogram;
inline GLuint projectionLoc, modelviewLoc, modelLoc, viewLoc, render_modeLoc;

inline GLuint lightcol;
inline GLuint lightpos;
inline GLuint numusedcol;
inline GLuint enablelighting;
inline GLuint ambientcol;
inline GLuint diffusecol;
inline GLuint specularcol;
inline GLuint emissioncol;
inline GLuint shininesscol;
inline GLuint camPosLoc;

inline GLuint lightsBufferID, lights_numberID, materialBufferID, objectNumberID;

// -------------- SHADER -------------- //
inline int lightsMaxNumber = 64;
inline int objectMaxNumber = 64;

inline enum { view2, translate, scale } transop;

inline float sx, sy;
inline float tx, ty;

// Globals regarding lighting details
inline constexpr int numLights = 5;
inline GLfloat lightposn[4 * numLights];
inline GLfloat lightcolor[4 * numLights];

inline GLfloat lightransf[4 * numLights];

// Callback and reshape globals
inline int render_mode = 0;
inline float current_vp_width = window_width/screenSeparation1;
inline float current_vp_height = window_height;

inline std::vector<float> actual_render(window_height*window_width*3);

inline int render_number = 0;

// -------------- SCENE -------------- //

inline Mesh flat_screen;

inline Camera mainCamera;
inline float cameraSpeed = 0.05f;

inline std::vector<Mesh*> scene_meshes;
inline std::vector<Light*> scene_lights;
inline std::vector<Object3D*> scene_objects;

inline glm::vec3 skyColor(0.6,0.8,1);

// -------------- IMGUI -------------- //

inline bool showMenus = true;
inline bool imguitest1;
inline ImGui::FileBrowser fileDialog;
inline int selected_object = 0;
inline int selected_light = 0;
inline ImGuiColorEditFlags colorEditFlag = (ImGuiColorEditFlags_NoOptions);
inline glm::vec3 old_light_color;
inline glm::vec3 old_mesh_color;
inline bool emissiveClick, transparentClick;

// --------------- OPENCL -------------- //
inline std::vector<float> gpuOutputImg;
inline size_t gpuOutputImg_size;

inline int meshNbr;
inline int lightNbr;
inline int textureNbr;
inline int texturesSize;

//Denoise stuff
inline int window_size = 5;
inline float denoise_bil_colordif_val = 1;
inline float denoise_bil_distance_val = 1;

inline std::vector<float> vertices_array;
inline std::vector<unsigned int> indices_array;
inline std::vector<unsigned int> splitMesh_array;
inline std::vector<unsigned int> splitMeshTri_array;
inline std::vector<float> materials_array;
inline std::vector<float> lights_array;
inline std::vector<float> bbox_array;
inline std::vector<unsigned char> texturesData_array;
inline std::vector<unsigned int >splitTexture_array;
inline std::vector<float> uv_array;
inline std::vector<unsigned int> splitUV_array;
#endif
