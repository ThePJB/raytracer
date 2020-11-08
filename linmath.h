#ifndef LINMATH_H
#define LINMATH_H

#include <math.h>
#include <stdio.h>

typedef union {
    double asArray[3];
    struct {
        double x;
        double y;
        double z;
    };
} vec3;

typedef union {
    double asArray[4];
    struct {
        double x;
        double y;
        double z;
        double w;
    };
} vec4;

typedef union {
    double asArray[16];
    double asDoubleArray[4][4];
} mat4;

typedef union {
    double asArray[9];
    double asDoubleArray[3][3];
} mat3;

vec3 new_vec3(double x, double y, double z);


double vec3_abs(vec3 a);
double dot(vec3 a, vec3 b);

vec3 normalize(vec3 a);
vec3 vec3_mul(vec3 a, double j);
vec3 vec3_div(vec3 a, double j);
vec3 vec3_add(vec3 a, vec3 b);
vec3 vec3_sub(vec3 a, vec3 b);
vec3 cross(vec3 a, vec3 b);
vec3 vec3_mul_vec3 (vec3 a, vec3 b); // elementwise
vec3 vec3_lerp(vec3 a, vec3 b, double j);

vec3 vec3_transform(mat4 transform, vec3 a);

vec3 normal_to_colour(vec3 a);

vec4 vec4_mul_mat4(mat4 t, vec4 a);
vec4 vec4_div(vec4 a, double j);

mat3 rx(double theta);
mat3 ry(double theta);
mat3 rz(double theta);
mat3 mat3_mul_mat3(mat3 a, mat3 b);

mat4 pad_to_mat4(mat3 a);
mat4 transformation_mat(double x, double y, double z, double roll, double pitch, double yaw, double scale);
mat4 mat4_mul(mat4 a, mat4 b);
mat4 mat4_transpose(mat4 a);
mat4 view_mat_lookat(vec3 pos, vec3 up, vec3 target);
mat4 make_perspective_mat(double fovx, double fovy,
    double znear, double zfar);

mat4 inverse_perspective_mat(double fovx, double fovy, 
    double znear, double zfar);

void print_mat4(mat4 a);

mat4 cam_mat_lookat(vec3 pos, vec3 up, vec3 target);












#endif