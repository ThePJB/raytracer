#ifndef VEC_H
#define VEC_H

typedef struct {
    double x, y, z;
} vec3;

double dot(vec3 a, vec3 b);
vec3 axpy(double a, vec3 x, vec3 y);
vec3 sub(vec3 x, vec3 y);
vec3 scale(double a, vec3 x);
vec3 proj(vec3 x, vec3 y);
vec3 make_unit(vec3 x);
void vec_str(char* buf, vec3 a);

#endif