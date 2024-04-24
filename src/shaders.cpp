#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>

#include "shaders.h"

std::string Shader::read_text_file(const char * filename){
    std::string str, ret = ""; 
    std::ifstream in; 
    in.open(filename); 

    if (in.is_open()){
        getline (in, str); 
        while (in){
            ret += str + "\n"; 
            getline (in, str); 
        }
        return ret; 
    }
    else{
        std::cerr << "Unable to Open File " << filename << "\n"; 
        throw 2; 
    }
}

void Shader::program_errors (const GLint program){
    GLint length; 
    GLchar * log; 
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length); 

    log = new GLchar[length+1];
    glGetProgramInfoLog(program, length, &length, log); 

    std::cout << "Compile Error, Log Below\n" << log << "\n"; 
    delete [] log; 
}

void Shader::shader_errors (const GLint shader){
    GLint length; 
    GLchar * log; 
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length); 

    log = new GLchar[length+1];
    glGetShaderInfoLog(shader, length, &length, log); 

    std::cout << "Compile Error, Log Below\n" << log << "\n"; 
    delete [] log; 
}

GLuint Shader::init_shaders (GLenum type, const char *filename){
    GLuint shader = glCreateShader(type);
    GLint compiled; 

    std::string str = read_text_file (filename); 
    const char * cstr = str.c_str();

    glShaderSource (shader, 1, &cstr, NULL); 
    glCompileShader (shader); 
    glGetShaderiv (shader, GL_COMPILE_STATUS, &compiled); 

    if (!compiled){ 
        shader_errors (shader); 
        throw 3; 
    }
    return shader; 
}

GLuint Shader::init_program (GLuint vertexShader_, GLuint geometryShader_, GLuint fragmentShader_){
    GLuint program = glCreateProgram(); 
    GLint linked; 

    glAttachShader(program, vertexShader_);
    glAttachShader(program, geometryShader_);
    glAttachShader(program, fragmentShader_);

    glLinkProgram(program); 
    glGetProgramiv(program, GL_LINK_STATUS, &linked); 

    if (linked){
        glDetachShader(program, vertexShader_);
        glDetachShader(program, geometryShader_);
        glDetachShader(program, fragmentShader_);
        glUseProgram(program);
    }
    else{ 
        program_errors(program); 
        throw 4; 
    }
    return program; 
}

GLuint Shader::init_program (GLuint vertexShader_, GLuint fragmentShader_){
    GLuint program = glCreateProgram();
    GLint linked;

    glAttachShader(program, vertexShader_);
    glAttachShader(program, fragmentShader_);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if (linked){
        glDetachShader(program, vertexShader_);
        glDetachShader(program, fragmentShader_);
        glUseProgram(program);
    }
    else{
        program_errors(program);
        throw 4;
    }
    return program;
}