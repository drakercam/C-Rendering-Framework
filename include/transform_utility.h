#ifndef TRANSFORM_UTILITY_H
#define TRANSFORM_UTILITY_H

#include "math_utility.h"
#include "stack_utility.h"

typedef struct
{
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;

} Transform;

STACK_DEFINE(Matrix4, MatrixStack, 256);

static MatrixStack stack;
static Matrix4 current_mat4 = Math_Mat4Identity();

static inline void Transform_PushMatrix()
{
    MatrixStack_Push(&stack, current_mat4);
}

static inline void Transform_PopMatrix()
{
    if (!MatrixStack_IsEmpty(&stack))
    {
        current_mat4 = MatrixStack_Pop(&stack);
    }
    else
    {
        printf("Pop called on empty matrix stack!\n");
    }
}

static inline void Transform_Translate(Vector3 t)
{
    current_mat4 = Math_Mat4Multiply(current_mat4, Math_Mat4Translate(t));
}

static inline void Transform_Rotate(float thetaRads, Vector3 axis)
{
    current_mat4 = Math_Mat4Multiply(current_mat4, Math_Mat4Rotate(Math_DegToRad(thetaRads), axis));
}

static inline void Transform_RotateX(float thetaRads)
{
    current_mat4 = Math_Mat4Multiply(current_mat4, Math_Mat4RotateX(Math_DegToRad(thetaRads)));
}

static inline void Transform_RotateY(float thetaRads)
{
    current_mat4 = Math_Mat4Multiply(current_mat4, Math_Mat4RotateY(Math_DegToRad(thetaRads)));
}

static inline void Transform_RotateZ(float thetaRads)
{
    current_mat4 = Math_Mat4Multiply(current_mat4, Math_Mat4RotateZ(Math_DegToRad(thetaRads)));
}

static inline void Transform_Scale(Vector3 s)
{
    current_mat4 = Math_Mat4Multiply(current_mat4, Math_Mat4Scale(s));
}

static inline Matrix4 Transform_ModelMatrix() { return current_mat4; }

static inline void Transform_LoadIdentity() { current_mat4 = Math_Mat4Identity(); }

#endif