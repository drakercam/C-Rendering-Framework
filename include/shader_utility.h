#ifndef SHADER_UTILITY_H
#define SHADER_UTILITY_H

#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "string_utility.h"
#include "file_utility.h"
#include "math_utility.h"

typedef struct
{
    unsigned int program;

} Shader;

static inline void Shader_CompileErrors(unsigned int shader, unsigned int type)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
        {
            printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
        }
        else if (type == GL_FRAGMENT_SHADER)
        {
            printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
        }
    }
}

static inline void Shader_LinkErrors(unsigned int program)
{
    int success;
    char infoLog[512];

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
    }
}

static inline void Shader_Create(Shader* shader, const char* vs_file, const char* fs_file)
{
    shader->program = 0;

    String vertex_program = File_Load(vs_file);
    String frag_program = File_Load(fs_file);

    const char* vp = vertex_program.data;
    const char* fp = frag_program.data;

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vp, NULL);
    glCompileShader(vertex_shader);
    // check compile errors
    Shader_CompileErrors(vertex_shader, GL_VERTEX_SHADER);

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fp, NULL);
    glCompileShader(fragment_shader);   
    Shader_CompileErrors(fragment_shader, GL_FRAGMENT_SHADER);

    shader->program = glCreateProgram();
    glAttachShader(shader->program, vertex_shader);
    glAttachShader(shader->program, fragment_shader);
    glLinkProgram(shader->program);
    Shader_LinkErrors(shader->program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    String_Free(&vertex_program);
    String_Free(&frag_program);
}   

static inline void Shader_SetUniform1i(Shader* shader, const char *name, int value)
{

    glUniform1i(glGetUniformLocation(shader->program, name), value);
}


static inline void Shader_SetUniform1f(Shader* shader, const char *name, float value)
{

    glUniform1f(glGetUniformLocation(shader->program, name), value);
}


static inline void Shader_SetUniform2f(Shader* shader, const char *name, const Vector2 vector)
{

    glUniform2f(glGetUniformLocation(shader->program, name), vector.x, vector.y);
}


static inline void Shader_SetUniform3f(Shader* shader, const char *name, const Vector3 vector)
{
    glUniform3f(glGetUniformLocation(shader->program, name), vector.x, vector.y, vector.z);
}


static inline void Shader_SetUniform4f(Shader* shader, const char *name, const Vector4 vector)
{
    glUniform4f(glGetUniformLocation(shader->program, name), vector.x, vector.y, vector.z, vector.w);
}


static inline void Shader_SetUniformMat4(Shader* shader, const char *name, const Matrix4 matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(shader->program, name), 1, GL_FALSE, matrix.m);
}

static inline void Shader_SetUniformIntArray(Shader* shader, const char *name, int len, const int *data)
{
    glUniform1iv(glGetUniformLocation(shader->program, name), len, data);
}

static inline void Shader_Enable(const Shader* shader)
{
    glUseProgram(shader->program);
}

static inline void Shader_Disable()
{
    glUseProgram(0);
}

static inline void Shader_Delete(Shader* shader)
{
    if (shader->program != 0)
    {
        glDeleteProgram(shader->program);
        shader->program = 0;
    }
}

#endif