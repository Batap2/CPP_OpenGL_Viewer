#ifndef SHADERS_H
#define SHADERS_H

#include <string>

class Shader{
public:
    std::string read_text_file(const char * filename);

    void program_errors (const GLint program);
    void shader_errors (const GLint shader);
    
    GLuint init_shaders (GLenum type, const char * filename);
    GLuint init_program (GLuint vertexshader, GLuint geometryShader, GLuint fragmentshader);


};

#endif
