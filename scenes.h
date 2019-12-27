#ifndef SCENES_H
#define SCENES_H

#include "shapes.h"

typedef struct {
    object* objects;
    int numObjects;
    vec3 bg_colour;
} scene;

void reflecto_test(scene* scene);
void bigger_test(scene* scene);

#endif