#ifndef CAMERA_UTILITY_H
#define CAMERA_UTILITY_H

#include "math_utility.h"
#include "input_utility.h"
#include "window_utility.h"

#include <stdbool.h>

typedef struct
{
    float speed;
    float rotationSpeed;
    Vector3 position;
    Quaternion orientation;

    Vector3 BASE_SIDE;
    Vector3 BASE_UP;
    Vector3 BASE_FORWARD;
    
} Camera3D;

/* -------------------------------------------------------------------------- */
/*                          3D CAMERA FUNCTIONS                               */
/* -------------------------------------------------------------------------- */

// default camera initialization
static inline void Camera3D_CreateDefault(Camera3D* camera)
{
    camera->speed = 0.0f;
    camera->rotationSpeed = Math_DegToRad(45.0f);
    camera->position = (Vector3){0.0f,0.0f,0.0f};
    camera->orientation = Math_QuatIdentity();
    camera->BASE_SIDE = (Vector3){1.0f,0.0f,0.0f};
    camera->BASE_UP = (Vector3){0.0f,1.0f,0.0f};
    camera->BASE_FORWARD = (Vector3){0.0f,0.0f,1.0f};
}

// initialize the camera with speed and rotation speed
static inline void Camera3D_Create(Camera3D* camera, float speed, float theta, Vector3 position)
{
    camera->speed = speed;
    camera->rotationSpeed = Math_DegToRad(theta);
    camera->position = position;
    camera->orientation = Math_QuatIdentity();
    camera->BASE_SIDE = (Vector3){1.0f,0.0f,0.0f};
    camera->BASE_UP = (Vector3){0.0f,1.0f,0.0f};
    camera->BASE_FORWARD = (Vector3){0.0f,0.0f,1.0f};
}

static inline Vector3 Camera3D_GetForward(const Camera3D* camera)
{
    Vector3 forward = Math_Vec3Normalize(
        Math_Mat3MultiplyVec3(Math_QuatConvertToMat3(camera->orientation), camera->BASE_FORWARD));

    return (Vector3){-forward.x, -forward.y, -forward.z};
}

static inline Vector3 Camera3D_GetSide(const Camera3D* camera)
{
    Vector3 forward = Math_Vec3Normalize(
        Math_Mat3MultiplyVec3(Math_QuatConvertToMat3(camera->orientation), camera->BASE_FORWARD));
    Vector3 up = Math_Vec3Normalize(
        Math_Mat3MultiplyVec3(Math_QuatConvertToMat3(camera->orientation), camera->BASE_UP));
    Vector3 side = Math_Vec3Normalize(Math_Vec3Cross(up, forward));

    return side;
}

static inline Vector3 Camera3D_GetUp(const Camera3D* camera)
{
    Vector3 forward = Math_Vec3Normalize(
        Math_Mat3MultiplyVec3(Math_QuatConvertToMat3(camera->orientation), camera->BASE_FORWARD));
    Vector3 side = Math_Vec3Normalize(
        Math_Mat3MultiplyVec3(Math_QuatConvertToMat3(camera->orientation), camera->BASE_SIDE));
    Vector3 up = Math_Vec3Normalize(Math_Vec3Cross(side, forward));

    return up;
}

static inline void Camera3D_Pitch(Camera3D* camera, float theta)  // rotate about x axis
{
    Vector3 axis = Math_Vec3Normalize(Camera3D_GetSide(camera));
    Quaternion rotQuat = Math_QuatRotate(axis, theta);
    camera->orientation = Math_QuatNormalize(Math_QuatMultiply(rotQuat, camera->orientation));
}

static inline void Camera3D_Yaw(Camera3D* camera, float theta)    // rotate about y axis
{
    Vector3 axis = Math_Vec3Normalize(Camera3D_GetUp(camera));
    Quaternion rotQuat = Math_QuatRotate(axis, theta);
    camera->orientation = Math_QuatNormalize(Math_QuatMultiply(rotQuat, camera->orientation));
}

static inline void Camera3D_Roll(Camera3D* camera, float theta)   // rotate about z axis
{
    Vector3 axis = Math_Vec3Normalize(Camera3D_GetForward(camera));
    Quaternion rotQuat = Math_QuatRotate(axis, theta);
    camera->orientation = Math_QuatNormalize(Math_QuatMultiply(rotQuat, camera->orientation));
}

static inline Matrix4 Camera3D_ViewMatrix(const Camera3D* camera)
{
    // Convert orientation quaternion to a 4x4 rotation matrix
    Matrix4 rotationMat = Math_QuatConvertToMat4(camera->orientation);

    // Transpose rotation (invert rotation)
    Matrix4 rotationInv = {
        rotationMat.m[0], rotationMat.m[4], rotationMat.m[8], 0.0f,
        rotationMat.m[1], rotationMat.m[5], rotationMat.m[9], 0.0f,
        rotationMat.m[2], rotationMat.m[6], rotationMat.m[10], 0.0f,
        0.0f,                0.0f,                0.0f,               1.0f
    };

    // Translation by negative camera position
    Vector3 negative_cam_pos = (Vector3){-camera->position.x, -camera->position.y, -camera->position.z};
    Matrix4 translation = Math_Mat4Translate(negative_cam_pos);

    return Math_Mat4Multiply(rotationInv, translation);
}

static inline void Camera3D_Update(Window* window, Camera3D* camera, float dt)
{
    Vector3 moveDir = (Vector3){0.0f, 0.0f, 0.0f};

    if (IsKeyPressed(window, GLFW_KEY_W)) moveDir = Math_Vec3Add(moveDir, Camera3D_GetForward(camera));
    if (IsKeyPressed(window, GLFW_KEY_S)) moveDir = Math_Vec3Add(moveDir, Math_Vec3Scale(Camera3D_GetForward(camera), -1.0f));

    if (IsKeyPressed(window, GLFW_KEY_A)) moveDir = Math_Vec3Add(moveDir, Math_Vec3Scale(Camera3D_GetSide(camera), -1.0f));
    if (IsKeyPressed(window, GLFW_KEY_D)) moveDir = Math_Vec3Add(moveDir, Camera3D_GetSide(camera));

    if (IsKeyPressed(window, GLFW_KEY_Q)) moveDir = Math_Vec3Add(moveDir, Math_Vec3Scale(Camera3D_GetUp(camera), -1.0f));
    if (IsKeyPressed(window, GLFW_KEY_E)) moveDir = Math_Vec3Add(moveDir, Camera3D_GetUp(camera));

    if (Math_Vec3Length(moveDir) > 0.0f)
    {
        moveDir = Math_Vec3Normalize(moveDir);
        camera->position = Math_Vec3Add(camera->position, Math_Vec3Scale(moveDir, camera->speed * dt));
    }

    float rotationAmt = camera->rotationSpeed * dt;

    // Pitch
    if (IsKeyPressed(window, GLFW_KEY_I))
        Camera3D_Pitch(camera, rotationAmt);

    if (IsKeyPressed(window, GLFW_KEY_K))
        Camera3D_Pitch(camera, -rotationAmt);

    // Yaw
    if (IsKeyPressed(window, GLFW_KEY_J))
        Camera3D_Yaw(camera, -rotationAmt);

    if (IsKeyPressed(window, GLFW_KEY_L))
        Camera3D_Yaw(camera, rotationAmt);

    // // Roll
    // if (IsKeyPressed(window, GLFW_KEY_U))
    //     Camera3D_Roll(camera, -rotationAmt);

    // if (IsKeyPressed(window, GLFW_KEY_O))
    //     Camera3D_Roll(camera, rotationAmt);

    camera->orientation = Math_QuatNormalize(camera->orientation);
}

typedef struct
{
    Vector2 position;
    float zoom;
    float rotation;
    float speed;

} Camera2D;

/* -------------------------------------------------------------------------- */
/*                           2D CAMERA FUNCTIONS                              */
/* -------------------------------------------------------------------------- */

static inline void Camera2D_CreateDefault(Camera2D* cam)
{
    cam->position = (Vector2){0.0f, 0.0f};
    cam->zoom = 1.0f;
    cam->rotation = 0.0f;
    cam->speed = 5.0f;
}

static inline void Camera2D_Create(Camera2D* cam, float speed, Vector2 position, float zoom, float rotation)
{
    cam->speed = speed;
    cam->position = position;
    cam->zoom = zoom;
    cam->rotation = rotation;
}

static inline Matrix4 Camera2D_ViewMatrix(Camera2D* cam)
{
    Vector3 negative_cam_pos = (Vector3){-cam->position.x, -cam->position.y, 0.0f};
    Matrix4 translate = Math_Mat4Translate(negative_cam_pos);

    Matrix4 rotate = Math_Mat4RotateZ(-cam->rotation);
    
    Vector3 camera_scale = (Vector3){cam->zoom, cam->zoom, 1.0f};
    Matrix4 scale = Math_Mat4Scale(camera_scale);

    Matrix4 scale_rotation = Math_Mat4Multiply(scale, rotate);
    return Math_Mat4Multiply(scale_rotation, translate);
}

static inline void Camera2D_Follow(Camera2D* cam, Vector2 target, float lerp_factor)
{
    cam->position.x += (target.x - cam->position.x) * lerp_factor;
    cam->position.y += (target.y - cam->position.y) * lerp_factor;
}

static inline void Camera2D_Update(const Window* window, Camera2D* cam, float dt, bool to_move, bool to_zoom, bool to_rotate)
{
    Vector2 moveDir = (Vector2){0.0f, 0.0f};
    float moveSpeed = cam->speed * dt; // adjust speed to your liking

    if (to_move)
    {
        if (IsKeyPressed(window, GLFW_KEY_S) || IsKeyPressed(window, GLFW_KEY_DOWN)) moveDir.y -= 1.0f;
        if (IsKeyPressed(window, GLFW_KEY_A) || IsKeyPressed(window, GLFW_KEY_LEFT)) moveDir.x -= 1.0f;
        if (IsKeyPressed(window, GLFW_KEY_D) || IsKeyPressed(window, GLFW_KEY_RIGHT)) moveDir.x += 1.0f;
        if (IsKeyPressed(window, GLFW_KEY_W) || IsKeyPressed(window, GLFW_KEY_UP)) moveDir.y += 1.0f;
    }

    if (Math_Vec2Length(moveDir) > 0.0f)
    {
        moveDir = Math_Vec2Normalize(moveDir);
        cam->position.x += moveDir.x * moveSpeed;
        cam->position.y += moveDir.y * moveSpeed;
    }

    // Zoom controls
    if (to_zoom)
    {
        if (IsKeyPressed(window, GLFW_KEY_E)) cam->zoom -= dt;       // zoom out
        if (cam->zoom < 0.1f) cam->zoom = 0.1f;                      // clamp
        if (IsKeyPressed(window, GLFW_KEY_Q)) cam->zoom += dt;       // zoom in
    }

    // Optional: rotation controls
    if (to_rotate)
    {
        float rotationSpeed = 1.0f * dt; // radians/sec
        if (IsKeyPressed(window, GLFW_KEY_Z)) cam->rotation += rotationSpeed;
        if (IsKeyPressed(window, GLFW_KEY_X)) cam->rotation -= rotationSpeed;
    }
}

#endif
