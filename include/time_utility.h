#ifndef TIME_UTILITY_H
#define TIME_UTILITY_H

#include <GLFW/glfw3.h>

namespace time_detail
{
    inline float lastFrame = 0.0f;
    inline float deltaTime = 0.0f;
    inline float totalTime = 0.0f;
}

// update the time
static inline void Time_Update()
{
    float now = static_cast<float>(glfwGetTime());
    time_detail::deltaTime = now - time_detail::lastFrame;
    time_detail::lastFrame = now;
    time_detail::totalTime += time_detail::deltaTime;
}

// get delta time
static inline float Time_Delta() { return time_detail::deltaTime; }

// get total time
static inline float Time_Total() { return time_detail::totalTime; }

#endif