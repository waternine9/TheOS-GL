#ifndef H_TOS_MATH
#define H_TOS_MATH

// Return inverse sqrt of x
float rsqrt(float x);

// Return sqrt of x
float sqrt(float x);

// Return sine of x
float sin(float x);

// Return cosine of x
float cos(float x);

// Return tangent of x
float tan(float x);

// Multiply two 4x4 matrices
void mat4_mul(float* A, float* B, float* out);

// Create a translation matrix
void mat4_translation(float x, float y, float z, float* out);

// Create a perspective projection matrix
void mat4_perspective(float fov, float aspect, float near, float far, float* out);

// Create a rotation matrix, angle is in degrees
void mat4_rotation(float angle, float x, float y, float z, float* out);

#endif // H_TOS_MATH