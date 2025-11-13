#ifndef TIME_UTILITY_H
#define TIME_UTILITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <GLFW/glfw3.h>

static float Time_LastFrame = 0.0f;
static float Time_DeltaTime = 0.0f;
static float Time_TotalTime = 0.0f;

// update the time
static inline void Time_Update()
{
    float now = (float)glfwGetTime();
    Time_DeltaTime = now - Time_LastFrame;
    Time_LastFrame = now;
    Time_TotalTime += Time_DeltaTime;
}

static inline float Time_Delta(void) { return Time_DeltaTime; }
static inline float Time_Total(void) { return Time_TotalTime; }

#ifdef __cplusplus
}
#endif

#endif