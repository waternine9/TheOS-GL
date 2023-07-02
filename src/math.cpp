#include "math.hpp"

#define PI 3.14159f

float rsqrt(float x)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5f;

    x2 = x * 0.5f;
    y  = x;
    i  = *(long*)&y;
    i  = 0x5f3759df - (i >> 1);
    y  = *(float*) &i;
    y  = y * (threehalfs - (x2 * y * y));
    y  = y * (threehalfs - (x2 * y * y));
    y  = y * (threehalfs - (x2 * y * y));

    return y;
}

float sqrt(float x)
{
    return rsqrt(x);
}

float sin(float x)
{
    x /= PI;
    int ix = x;
    float modx = x - ix;
    float result = modx - modx * modx;
    if (ix % 2) return result * -4.0f;
    return result * 4;
}

float cos(float x)
{
    return sin(x + PI / 2.0f);
}

float tan(float x)
{
    return sin(x) / cos(x);
}

// Multiply two 4x4 matrices
void mat4_mul(float* A, float* B, float* out) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            out[i*4 + j] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                out[i*4 + j] += A[i*4 + k] * B[k*4 + j];
            }
        }
    }
}

// Create a translation matrix
void mat4_translation(float x, float y, float z, float* out) {
    for (int i = 0; i < 16; ++i) {
        out[i] = 0.0f;
    }
    out[0] = out[5] = out[10] = out[15] = 1.0f;
    out[3] = x;
    out[7] = y;
    out[11] = z;
}

// Create a perspective projection matrix
void mat4_perspective(float fov, float aspect, float near, float far, float* out) {
    float range = tan(fov * 0.5f * PI / 180.0f) * near;
    float Sx = (1.0f / range) / aspect;
    float Sy = 1.0f / range;
    float Sz = -(far + near) / (far - near);
    float Pz = -(2.0f * far * near) / (far - near);

    for (int i = 0; i < 16; ++i) {
        out[i] = 0.0f;
    }

    out[0] = Sx;
    out[5] = Sy;
    out[10] = Sz;
    out[11] = Pz;
    out[14] = -1.0f;
}

// Create a rotation matrix (angle is in degrees)
void mat4_rotation(float angle, float x, float y, float z, float* out) {
    float radians = angle * PI / 180.0f;
    float c = cos(radians);
    float s = sin(radians);

    float magnitude = sqrt(x*x + y*y + z*z);
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;

    for (int i = 0; i < 16; ++i) {
        out[i] = 0.0f;
    }

    out[0] = x*x*(1-c)+c;
    out[1] = x*y*(1-c)-z*s;
    out[2] = x*z*(1-c)+y*s;
    out[4] = y*x*(1-c)+z*s;
    out[5] = y*y*(1-c)+c;
    out[6] = y*z*(1-c)-x*s;
    out[8] = z*x*(1-c)-y*s;
    out[9] = z*y*(1-c)+x*s;
    out[10] = z*z*(1-c)+c;
    out[15] = 1.0f;
}
