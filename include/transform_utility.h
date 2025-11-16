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

// declare globals
extern MatrixStack stack;
extern Matrix4 current_mat4;

void Transform_Init(void);
void Transform_PushMatrix(void);
void Transform_PopMatrix(void);
void Transform_Translate(Vector3 t);
void Transform_Rotate(float thetaRads, Vector3 axis);
void Transform_Scale(Vector3 s);
Matrix4 Transform_ModelMatrix(void);
void Transform_LoadIdentity(void);

#endif