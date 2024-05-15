cmake_minimum_required(VERSION 3.16)

project(Viewer)

# Définir les drapeaux de compilation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -std=c++17")

# Définir le répertoire des sources et include
set(source_dir "${PROJECT_SOURCE_DIR}/src")
set(include_dir "${PROJECT_SOURCE_DIR}/include")

# Définir le répertoire de sortie des exécutables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Rechercher les packages nécessaires
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_subdirectory(include)

# Ajouter les fichiers source
file(GLOB source_files "${source_dir}/*.cpp" "include/imgui/*.cpp" "include/ImGuizmo/*.cpp")

# Ajouter l'exécutable avec les fichiers source
add_executable(Viewer ${source_files}
    src/Light.h
    src/Material.h
    src/MeshLoader.h
    src/ShaderUtils.h
    src/SceneOperations.h
    src/Object3D.h
    src/Plane.h
    src/Triangle.h
    src/Sphere.h
    src/Square.h
)

target_link_directories(Viewer PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(Viewer PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_directories(Viewer PUBLIC ${PROJECT_SOURCE_DIR}/include/assimp/include)
target_include_directories(Viewer PUBLIC ${PROJECT_SOURCE_DIR}/include/assimp/include)


# Lier les bibliothèques nécessaires
if(WIN32)
    target_link_libraries(Viewer glfw3 GLEW32 opengl32 OpenGL::GL assimp)
endif (WIN32)

if(UNIX)
    target_link_libraries(Viewer glfw OpenGL::GL GLEW assimp)
endif (UNIX)
