# C++ OpenGL Viewer / Mesh Importer

## Libraries needed:

- Freeglut (windows)
- GLEW
- GLFW

## Usage

To compile, create a directory called `build` and create Makefile using the cmake build system

    mkdir build && cd build
    cmake ..
    make

Once compiled, launch the viewer and open a file with the "file" button

![](demo/openFile.gif)

## Input :
Translate camera : a z e q s d
Rotate camera : right clic + mouse
Change camera speed : scrollwheel

## Materials :

Materials can be modified by clicking on an object in the scene object list.

## Lights :

Lights can be added or added by clicking on a light in the scene lights list.

## Dependencies and Fork

This project is forked from https://github.com/Batap2/OmegaRayTrace-Ultimate---GPU-edition.
It using GLFW for the window, ImGui for the interface, Assimp, dirent & imgui-filebrowser for importing 3D files and glm.
