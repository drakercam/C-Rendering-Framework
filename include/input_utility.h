#ifndef INPUT_UTILITY_H
#define INPUT_UTILITY_H

#include <GLFW/glfw3.h>
#include "window_utility.h"

static inline bool IsKeyPressed(const Window& window, int key)
{
    return glfwGetKey(window.w, key) == GLFW_PRESS;
}

#endif