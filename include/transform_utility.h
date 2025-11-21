#ifndef TRANSFORM_UTILITY_H
#define TRANSFORM_UTILITY_H

/* C++ linkage */
#ifdef __cplusplus
extern "C" {
#endif

#include "math_utility.h"
#include "stack_utility.h"

/* ----- Types ----- */
typedef struct
{
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
} Transform;

/* ----- Stack type for matrices ----- */
STACK_DEFINE(Matrix4, MatrixStack, 256);

/* ----- Globals ----- */
extern MatrixStack stack;
extern Matrix4 current_mat4;

/* ----- Implementation ----- */

/* define globals once */
MatrixStack stack;
Matrix4 current_mat4;

/* ----- Functions ----- */
static inline void Transform_Init(void) 
{ 
    current_mat4 = Math_Mat4Identity(); 
}

static inline void Transform_PushMatrix(void) 
{ 
    MatrixStack_Push(&stack, current_mat4); 
}

static inline void Transform_PopMatrix(void) 
{ 
    if(!MatrixStack_IsEmpty(&stack)) 
        current_mat4 = MatrixStack_Pop(&stack); 
}

static inline void Transform_Translate(Vector3 t) 
{ 
    current_mat4 = Math_Mat4Multiply(current_mat4, Math_Mat4Translate(t)); 
}

static inline void Transform_Rotate(float thetaRads, Vector3 axis) 
{ 
    current_mat4 = Math_Mat4Multiply(current_mat4, Math_Mat4Rotate(thetaRads, axis)); 
}

static inline void Transform_Scale(Vector3 s) 
{ 
    current_mat4 = Math_Mat4Multiply(current_mat4, Math_Mat4Scale(s)); 
}

static inline Matrix4 Transform_ModelMatrix(void) 
{ 
    return current_mat4; 
}

static inline void Transform_LoadIdentity(void) 
{ 
    current_mat4 = Math_Mat4Identity(); 
}

/* ----- End of C++ linkage ----- */
#ifdef __cplusplus
}
#endif

#endif /* TRANSFORM_UTILITY_H */