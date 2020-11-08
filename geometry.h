#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "linmath.h"
#include <stdbool.h>


// Geometry and ray stuff goes here

typedef struct {
    vec3 origin;
    vec3 direction;
} ray;

typedef enum {
    OBJ_NONE,
    OBJ_SPHERE,
    OBJ_TRIANGLE,
    OBJ_MESH,
    NUM_OBJECTS,
} object_tag;

typedef struct {
    vec3 center;
    double radius;
} sphere;

typedef struct {
    vec3 vert[3];
} triangle;

typedef struct {
    int num_triangles;
    triangle *triangles;
} mesh;

typedef struct {
    object_tag tag;
    vec3 colour;
    union {
        sphere sphere;
        triangle triangle;
        mesh mesh;
    };
} object;

typedef struct {
    vec3 pos;
    vec3 colour;
    double intensity;
} point_light;


typedef struct {
    bool did_intersect;
    vec3 normal;
    vec3 position;
    double distance;
    vec3 colour;
} intersection_info;

intersection_info ray_intersects_object(ray r, object o);
mat4 ray_as_mat4(ray r);

ray transform_ray(mat4 transform, ray r);

void print_ray(ray r);

#endif