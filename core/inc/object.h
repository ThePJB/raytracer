#ifndef SHAPES_H
#define SHAPES_H

#include "vec.h"

typedef enum {
    NONE,
    SPHERE,
    PLANE,
    NUM_OBJECT_TYPES,
} object_type;

typedef struct {
    vec3 origin, direction;
} ray;

typedef struct {
    vec3 center;
    double radius;
} sphere;

typedef struct {
    vec3 center;
    vec3 normal;
} plane;

typedef struct {
    object_type type;
    vec3 colour;
    double reflectance;

    union {
        sphere sphere;
        plane plane;
    };

} object;

void object_str(char* buf, object object);
int ray_intersects_object(vec3 *normal, vec3 *position, double *dist, ray ray, object object);
ray ray_reflect(ray incident, vec3 location, vec3 normal);
void ray_str(char* buf, ray ray);
#endif