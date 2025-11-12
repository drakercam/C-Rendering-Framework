#ifndef WINDOW_UTILITY_H
#define WINDOW_UTILITY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "math_utility.h"
#include "time_utility.h"

static inline void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

typedef struct
{
    GLFWwindow* w;
    int width, height;
    std::string title;
    float fov;
    float aspect;

} Window;

static inline bool Window_Init(Window* window)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window->w = glfwCreateWindow(window->width, window->height, window->title.c_str(), NULL, NULL);
    if (window->w == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window->w);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window->w);
        glfwTerminate();
        return false;
    }

    glfwSetFramebufferSizeCallback(window->w, framebuffer_size_callback);

    return true;
}

static inline bool Window_Create(Window* window, int w, int h, float fov, std::string t)
{
    window->w = NULL;
    window->width = w;
    window->height = h;
    window->title = t;
    window->fov = Math_DegToRad(fov);
    window->aspect = float(window->width) / float(window->height);

    return Window_Init(window);
}

static inline bool Window_IsOpen(Window window)
{
    return !glfwWindowShouldClose(window.w);
}

static inline void Window_SwapBuffers(Window window)
{
    glfwSwapBuffers(window.w);
}

static inline void Window_PollEvents()
{
    glfwPollEvents();
}

static inline void Window_Delete()
{
    glfwTerminate();
}

static inline void Window_EnableDepthTest()
{
    glEnable(GL_DEPTH_TEST);
}

static inline void Window_DisableDepthTest()
{
    glDisable(GL_DEPTH_TEST);
}

static inline void Window_Clear(Vector4 color)
{
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static inline void Window_PrintFPS(void)
{
    printf("FPS: %.2f | DeltaTime: %.4f\n", 1.0f / Time_Delta(), Time_Delta());
}

#endif