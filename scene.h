#ifndef SCENE_H
#define SCENE_H

#include "linmath.h"
#include "geometry.h"

#define MAX_CAMERAS 5
#define MAX_OBJECTS 10
#define MAX_LIGHTS 5

typedef struct {
    mat4 cameras[MAX_CAMERAS];
    object objects[MAX_OBJECTS];
    point_light lights[MAX_LIGHTS];
} scene;

typedef enum {
    SHADING_AMBIENT,
    SHADING_FACING_RATIO,
    SHADING_NORMALS,
    SHADING_GOOCH,
    SHADING_LIGHTS,
} shading_mode;

#define MAX_DEPTH 15

// render scene
void render_scene(scene *s, char *filename, int w, int h, int camera_index,
     double fovx, double a, shading_mode shading_mode);

#endif