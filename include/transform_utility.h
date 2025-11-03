#ifndef TRANSFORM_UTILITY_H
#define TRANSFORM_UTILITY_H

#include "math_utility.h"
#include <stack>
#include <iostream>

typedef struct
{
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;

} Transform;

// to hide the details of the matrix stack
// mimics openframeworks
namespace transform_detail
{
    inline std::stack<Matrix4> stack;
    inline Matrix4 current_mat4 = Math_Mat4Identity();
}

static inline void Transform_PushMatrix()
{
    transform_detail::stack.push(transform_detail::current_mat4);
}

static inline void Transform_PopMatrix()
{
    if (!transform_detail::stack.empty())
    {
        transform_detail::current_mat4 = transform_detail::stack.top();
        transform_detail::stack.pop();
    }
    else
    {
        std::cerr << "Pop called on empty matrix stack!" << std::endl;
    }
}

static inline void Transform_Translate(const Vector3& t)
{
    transform_detail::current_mat4 = Math_Mat4Multiply(transform_detail::current_mat4, Math_Mat4Translate(t));
}

static inline void Transform_Rotate(float thetaRads, const Vector3& axis)
{
    transform_detail::current_mat4 = Math_Mat4Multiply(transform_detail::current_mat4, Math_Mat4Rotate(Math_DegToRad(thetaRads), axis));
}

static inline void Transform_RotateX(float thetaRads)
{
    transform_detail::current_mat4 = Math_Mat4Multiply(transform_detail::current_mat4, Math_Mat4RotateX(Math_DegToRad(thetaRads)));
}

static inline void Transform_RotateY(float thetaRads)
{
    transform_detail::current_mat4 = Math_Mat4Multiply(transform_detail::current_mat4, Math_Mat4RotateY(Math_DegToRad(thetaRads)));
}

static inline void Transform_RotateZ(float thetaRads)
{
    transform_detail::current_mat4 = Math_Mat4Multiply(transform_detail::current_mat4, Math_Mat4RotateZ(Math_DegToRad(thetaRads)));
}

static inline void Transform_Scale(const Vector3& s)
{
    transform_detail::current_mat4 = Math_Mat4Multiply(transform_detail::current_mat4, Math_Mat4Scale(s));
}

static inline const Matrix4& Transform_ModelMatrix() { return transform_detail::current_mat4; }

static inline void Transform_LoadIdentity() { transform_detail::current_mat4 = Math_Mat4Identity(); }

#endif