#ifndef SCENES_H
#define SCENES_H

#include "object.h"
#include "camera.h"

#define MAX_OBJECTS 100
#define MAX_CAMERAS 100

typedef struct {
    object objects[MAX_OBJECTS];
    camera cameras[MAX_CAMERAS];
    vec3 bg_colour;
} scene;

#endif