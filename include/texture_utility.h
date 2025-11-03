#ifndef TEXTURE_UTILITY_H
#define TEXTURE_UTILITY_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

typedef struct
{
    unsigned int id;
    std::string path;
    unsigned char* local_buffer;    // buffer data for the texture
    int width, height, bits_per_pixel;

} Texture;

static inline void Texture_Init(Texture& tex, std::string path)
{
    tex.path = path;
    tex.local_buffer = nullptr;
    tex.width = 0;
    tex.height = 0;
    tex.bits_per_pixel = 0;

    stbi_set_flip_vertically_on_load(true);
    tex.local_buffer = stbi_load(path.c_str(), &tex.width, &tex.height, &tex.bits_per_pixel, 4);
    if (!tex.local_buffer)
    {
        std::cout << "Failed to load texture: " << path << std::endl;
        return;
    }

    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tex.width, tex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.local_buffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (tex.local_buffer)
        stbi_image_free(tex.local_buffer);
}

static inline void Texture_Delete(Texture& tex)
{
    glDeleteTextures(1, &tex.id);
    tex.id = 0;
}

static inline void Texture_Enable(Texture& tex, unsigned int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, tex.id);
}

static inline void Texture_Disable(void)
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

#endif