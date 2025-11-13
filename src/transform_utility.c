#include "transform_utility.h"

MatrixStack stack;
Matrix4 current_mat4;

void Transform_Init(void) { current_mat4 = Math_Mat4Identity(); }
void Transform_PushMatrix(void) { MatrixStack_Push(&stack, current_mat4); }
void Transform_PopMatrix(void) { if(!MatrixStack_IsEmpty(&stack)) current_mat4 = MatrixStack_Pop(&stack); }
void Transform_Translate(Vector3 t) { current_mat4 = Math_Mat4Multiply(current_mat4, Math_Mat4Translate(t)); }
void Transform_Rotate(float thetaRads, Vector3 axis) { current_mat4 = Math_Mat4Multiply(current_mat4, Math_Mat4Rotate(thetaRads, axis)); }
void Transform_Scale(Vector3 s) { current_mat4 = Math_Mat4Multiply(current_mat4, Math_Mat4Scale(s)); }
Matrix4 Transform_ModelMatrix(void) { return current_mat4; }
void Transform_LoadIdentity(void) { current_mat4 = Math_Mat4Identity(); }