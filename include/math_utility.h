#ifndef MATH_UTILITY_H
#define MATH_UTILITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>

#define PI 3.14159265358979323846f

// --- Vector Operations ---

typedef struct
{
    float x,y;

} Vector2;

typedef struct
{
    float x,y,z;

} Vector3;

typedef struct
{
    float x,y,z,w;

} Vector4;

static inline Vector2 Math_Vec2Add(const Vector2 v1, const Vector2 v2) { return (Vector2){v1.x + v2.x, v1.y + v2.y}; }

static inline Vector3 Math_Vec3Add(const Vector3 v1, const Vector3 v2){ return (Vector3){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}; }

static inline Vector4 Math_Vec4Add(const Vector4 v1, const Vector4 v2) { return (Vector4){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w}; }

static inline Vector2 Math_Vec2Sub(const Vector2 v1, const Vector2 v2) { return (Vector2){v1.x - v2.x, v1.y - v2.y};  }

static inline Vector3 Math_Vec3Sub(const Vector3 v1, const Vector3 v2) { return (Vector3){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z}; }

static inline Vector4 Math_Vec4Sub(const Vector4 v1, const Vector4 v2) { return (Vector4){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w}; }

static inline Vector2 Math_Vec2Scale(const Vector2 v, float s) { return (Vector2){ s*v.x, s*v.y }; }

static inline Vector3 Math_Vec3Scale(const Vector3 v, float s) { return (Vector3){ s*v.x, s*v.y, s*v.z }; }

static inline Vector4 Math_Vec4Scale(const Vector4 v, float s) { return (Vector4){ s*v.x, s*v.y, s*v.z, s*v.w }; }

static inline float Math_Vec2Length(const Vector2 v) { return sqrtf(v.x*v.x + v.y*v.y); }

static inline float Math_Vec3Length(const Vector3 v) { return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z); }

static inline float Math_Vec4Length(const Vector4 v) { return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w); }

static inline float Math_Vec2Dot(const Vector2 v1, const Vector2 v2) { return (v1.x*v2.x + v1.y*v2.y); }

static inline float Math_Vec3Dot(const Vector3 v1, const Vector3 v2) { return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z); }

static inline Vector3 Math_Vec3Cross(const Vector3 v1, const Vector3 v2) { return (Vector3){ (v1.y*v2.z - v1.z*v2.y), (v1.z*v2.x - v1.x*v2.z), (v1.x*v2.y - v1.y*v2.x) }; }

static inline Vector2 Math_Vec2Normalize(const Vector2 v)
{
    float length = Math_Vec2Length(v);
    if (length == 0.0f) return (Vector2){0,0};
    return (Vector2){ (v.x/length), (v.y/length) };
}

static inline Vector3 Math_Vec3Normalize(const Vector3 v)
{
    float length = Math_Vec3Length(v);
    if (length == 0.0f) return (Vector3){0,0,0};
    return (Vector3){ (v.x/length), (v.y/length), (v.z/length) };
}

static inline float Math_Vec2Distance(const Vector2 v1, const Vector2 v2)
{
    return sqrtf(((v1.x-v2.x)*(v1.x-v2.x)) + ((v1.y-v2.y)*(v1.y-v2.y)));
}

static inline float Math_Vec3Distance(const Vector3 v1, const Vector3 v2)
{
    return sqrtf(((v1.x-v2.x)*(v1.x-v2.x)) + ((v1.y-v2.y)*(v1.y-v2.y)) + ((v1.z-v2.z)*(v1.z-v2.z)));
}

static inline float Math_Vec2DistanceSq(const Vector2 v1, const Vector2 v2)
{
    return ((v1.x-v2.x)*(v1.x-v2.x)) + ((v1.y-v2.y)*(v1.y-v2.y));
}

static inline float Math_Vec3DistanceSq(const Vector3 v1, const Vector3 v2)
{
    return ((v1.x-v2.x)*(v1.x-v2.x)) + ((v1.y-v2.y)*(v1.y-v2.y)) + ((v1.z-v2.z)*(v1.z-v2.z));
}

static inline Vector2 Math_Vec2Lerp(const Vector2 v1, const Vector2 v2, float t)
{
    Vector2 diff = Math_Vec2Sub(v2, v1);
    Vector2 scaled = Math_Vec2Scale(diff, t);
    Vector2 result = Math_Vec2Add(v1, scaled);
    return result;
}

static inline Vector3 Math_Vec3Lerp(const Vector3 v1, const Vector3 v2, float t)
{
    Vector3 diff = Math_Vec3Sub(v2, v1);
    Vector3 scaled = Math_Vec3Scale(diff, t);
    Vector3 result = Math_Vec3Add(v1, scaled);
    return result;
}

// -------------------------

// --- Other Math related functions ---

static inline float Math_DegToRad(const float degrees)
{
    return (degrees * PI / 180.0f);
}

static inline float Math_Clamp(float x, float min, float max)
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

static inline Vector2 Math_Vec2Clamp(const Vector2 v, const Vector2 min, const Vector2 max)
{
    return (Vector2){Math_Clamp(v.x, min.x, max.x), Math_Clamp(v.y, min.y, max.y)};
}

static inline Vector3 Math_Vec3Clamp(const Vector3 v, const Vector3 min, const Vector3 max)
{
    return (Vector3){Math_Clamp(v.x, min.x, max.x), Math_Clamp(v.y, min.y, max.y), Math_Clamp(v.z, min.z, max.z)};
}

// ------------------------------------

// --- Matrix Operations ---*
typedef struct 
{ 
    float m[9];     // Column major for opengl

} Matrix3;

static inline Matrix3 Math_Mat3Identity()
{
    return (Matrix3){
        1,0,0,
        0,1,0,
        0,0,1
    };
}

static inline Vector3 Math_Mat3MultiplyVec3(const Matrix3 m, const Vector3 v)
{
    return (Vector3){
        m.m[0]*v.x + m.m[3]*v.y + m.m[6]*v.z,
        m.m[1]*v.x + m.m[4]*v.y + m.m[7]*v.z,
        m.m[2]*v.x + m.m[5]*v.y + m.m[8]*v.z
    };
}

static inline Matrix3 Math_Mat3Multiply(const Matrix3 a, const Matrix3 b)
{
    Matrix3 res = {0};

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            res.m[col*3 + row] =
                a.m[0*3 + row] * b.m[col*3 + 0] +
                a.m[1*3 + row] * b.m[col*3 + 1] +
                a.m[2*3 + row] * b.m[col*3 + 2];
        }
    }

    return res;
}

static inline Matrix3 Math_Mat3Rotate(float theta, const Vector3 axis)
{
    Vector3 n = Math_Vec3Normalize(axis);
    float c = cosf(theta);
    float s = sinf(theta);
    float ic = 1.0f - c;

    return (Matrix3){
        c + n.x*n.x*ic,      n.x*n.y*ic - n.z*s,  n.x*n.z*ic + n.y*s,
        n.y*n.x*ic + n.z*s,  c + n.y*n.y*ic,      n.y*n.z*ic - n.x*s,
        n.z*n.x*ic - n.y*s,  n.z*n.y*ic + n.x*s,  c + n.z*n.z*ic
    };
}

typedef struct 
{ 
    float m[16];     // Column major for opengl
    
} Matrix4;

static inline Matrix4 Math_Mat4Identity() 
{ 
    return (Matrix4){
        1,0,0,0,    // column 1
        0,1,0,0,    // column 2
        0,0,1,0,    // column 3
        0,0,0,1     // column 4
    }; 
}

static inline Matrix4 Math_Mat4Translate(const Vector3 t)
{
    return (Matrix4){
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        t.x,t.y,t.z,1
    };
}

static inline Matrix4 Math_Mat4Scale(const Vector3 s)
{
    return (Matrix4){
        s.x,0,0,0,
        0,s.y,0,0,
        0,0,s.z,0,
        0,0,0,1
    };
}
static inline Matrix4 Math_Mat4Rotate(float theta, const Vector3 axis)
{
    // theta is expected in radians
    Vector3 nA = Math_Vec3Normalize(axis);
    float c = cosf(theta);
    float s = sinf(theta);
    float ic = (1-c);

    return (Matrix4){
        c+(nA.x*nA.x)*ic,      nA.y*nA.x*ic+nA.z*s,    nA.z*nA.x*ic-nA.y*s,    0,  // Col 1
        nA.x*nA.y*ic-nA.z*s,   c+(nA.y*nA.y)*ic,       nA.y*nA.z*ic+nA.x*s,    0,  // Col 2
        nA.x*nA.z*ic+nA.y*s,   nA.y*nA.z*ic-nA.x*s,    c+(nA.z*nA.z)*ic,       0,  // Col 3
        0,                     0,                      0,                      1   // Col 4
    };
}

static inline Matrix4 Math_Mat4RotateX(float theta)
{
    // Theta expected in rads
    float c = cosf(theta);
    float s = sinf(theta);

    return (Matrix4){
        1, 0, 0, 0,
        0, c, -s, 0,
        0, s, c, 0,
        0, 0, 0, 1
    };
}

static inline Matrix4 Math_Mat4RotateY(float theta)
{
    // Theta expected in rads
    float c = cosf(theta);
    float s = sinf(theta);

    return (Matrix4){
        c, 0, s, 0,
        0, 1, 0, 0,
        -s, 0, c, 0,
        0, 0, 0, 1
    };
}

static inline Matrix4 Math_Mat4RotateZ(float theta)
{
    // Theta expected in rads
    float c = cosf(theta);
    float s = sinf(theta);

    return (Matrix4){
        c, -s, 0, 0,
        s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
}
static inline Matrix4 Math_Mat4Multiply(const Matrix4 m1, const Matrix4 m2)
{
    Matrix4 res = {0};

    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            for (int k = 0; k < 4; ++k)
            {
                res.m[col*4 + row] += m1.m[k*4+row] * m2.m[col*4+k];
            }
        }
    }

    return res;
}

static inline Matrix4 Math_GetProjMatrix(float fovRadians, float aspect, float nearPlane, float farPlane)
{
    Matrix4 result = Math_Mat4Identity();

    float tanHalfFOV = tanf(fovRadians * 0.5f);
    float zRange = farPlane - nearPlane;

    result.m[0]  = 1.0f / (tanHalfFOV * aspect);
    result.m[5]  = 1.0f / tanHalfFOV;
    result.m[10] = -(farPlane + nearPlane) / zRange;
    result.m[11] = -1.0f;
    result.m[14] = -2.0f * farPlane * nearPlane / zRange;
    result.m[15] = 0.0f;

    return result;
}

static inline Matrix4 Math_GetOrthoMatrix(float l, float r, float b, float t, float near, float far)
{
    Matrix4 result = Math_Mat4Identity();
    
    result.m[0]  = 2.0f / (r - l);
    result.m[5]  = 2.0f / (t - b);
    result.m[10] = -2.0f / (far - near);
    result.m[12] = -((r + l) / (r - l));
    result.m[13] = -((t + b) / (t - b));
    result.m[14] = -((far + near) / (far - near));

    return result;
}

// -------------------------

// --- Quaternion Operations

typedef struct
{
    float w, x, y, z;

} Quaternion;

static inline Quaternion Math_QuatIdentity(void)
{
    return (Quaternion){1.0f, 0.0f, 0.0f, 0.0f};
}

static inline float Math_QuatLength(const Quaternion q)
{
    return sqrtf(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
}

static inline Quaternion Math_QuatNormalize(const Quaternion q)
{
    float length = Math_QuatLength(q);
    if (length <= 0.0f) return Math_QuatIdentity();
    return (Quaternion){ q.w/length, q.x/length, q.y/length, q.z/length };
}

static inline Quaternion Math_QuatRotate(const Vector3 axis, float thetaRads)
{
    Vector3 normAxis = Math_Vec3Normalize(axis);
    float half = thetaRads * 0.5f;
    return (Quaternion){
        cosf(half),
        normAxis.x * sinf(half),
        normAxis.y * sinf(half),
        normAxis.z * sinf(half)
    };
}

static inline Quaternion Math_QuatMultiply(const Quaternion q1, const Quaternion q2)
{
    Quaternion result = (Quaternion){
        q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z,
        q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y,
        q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x,
        q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w
    };

    return Math_QuatNormalize(result);
}

static inline Matrix4 Math_QuatConvertToMat4(const Quaternion q)
{
    float xx = q.x*q.x;
    float yy = q.y*q.y;
    float zz = q.z*q.z;

    return (Matrix4){
        (1-2*(yy) - 2*(zz)), (2*q.x*q.y + 2*q.w*q.z), (2*q.x*q.z - 2*q.w*q.y),   0,
        (2*q.x*q.y - 2*q.w*q.z), (1 - 2*(xx) - 2*(zz)), (2*q.y*q.z + 2*q.w*q.x), 0,
        (2*q.x*q.z + 2*q.w*q.y), (2*q.y*q.z - 2*q.w*q.x), (1 - 2*(xx) - 2*(yy)), 0,
        0,                            0,                               0,                1
    };  
}

static inline Matrix3 Math_QuatConvertToMat3(const Quaternion q)
{
    float xx = q.x*q.x;
    float yy = q.y*q.y;
    float zz = q.z*q.z;

    return (Matrix3) {
        (1-2*(yy) - 2*(zz)), (2*q.x*q.y + 2*q.w*q.z), (2*q.x*q.z - 2*q.w*q.y),
        (2*q.x*q.y - 2*q.w*q.z), (1 - 2*(xx) - 2*(zz)), (2*q.y*q.z + 2*q.w*q.x),
        (2*q.x*q.z + 2*q.w*q.y), (2*q.y*q.z - 2*q.w*q.x), (1 - 2*(xx) - 2*(yy))
    };
}

// -------------------------

#ifdef __cplusplus
}
#endif

#endif