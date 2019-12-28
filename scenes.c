#include "scenes.h"
#include "shapes.h"

#include <stdlib.h>

void reflecto_test(scene* scene) {
    int numObjects = 2;
    object* objects = calloc(numObjects, sizeof(object));

    objects[0] = (object) {
                .colour = (vec3) {0.0, 1.0, 0.0},
        .type = SPHERE,
        .sphere = (sphere) {
            .center = (vec3) {0,-3,0},
            .radius = 1,
        }
    };
    
    objects[1] = (object) {
        .colour = (vec3) {0.7, 0.7, 0.2},
        .type = SPHERE,
        .reflectance=1,
        .sphere = (sphere) {
            .center = (vec3) {0,3, 0},
            .radius = 1,
        }
    };

    scene->bg_colour = (vec3){0.5,0,0.5};
    scene->numObjects = numObjects;
    scene->objects = objects;
}

void bigger_test(scene* scene) {
    int numObjects = 5;
    object* objects = calloc(numObjects, sizeof(object));

    objects[0] = (object) {
                .colour = (vec3) {0.0, 1.0, 0.0},
        .type = SPHERE,
        .sphere = (sphere) {
            .center = (vec3) {-1,0,5},
            .radius = 1,
        }
    };
    
    objects[1] = (object) {
        .colour = (vec3) {0.7, 0.7, 0.2},
        .type = SPHERE,
        .reflectance=0,
        .sphere = (sphere) {
            .center = (vec3) {1,0.5, 6},
            .radius = 1,
        }
    };

    objects[2] = (object) {
        .colour = (vec3) {1, 0, 0},
        .type = SPHERE,
        .sphere = (sphere) {
            .center = (vec3) {0.5,1.5, 5},
            .radius = 0.5,
        }
    };

    objects[3] = (object) {
        .colour = (vec3) {0.2, 0, 0.7},
        .type = SPHERE,
        .sphere = (sphere) {
            .center = (vec3) {-1,0, 3},
            .radius = 0.4,
        }
    };

    objects[4] = (object) {
        .colour = (vec3) {0,0,1},
        .type = PLANE,
        //.reflectance=1,
        .plane = (plane) {
            .center = (vec3) {0, 16, 0},
            .normal = (vec3) {0, -1, 0},
        }
    };

    scene->bg_colour = (vec3){0.5,0,0.5};
    scene->numObjects = numObjects;
    scene->objects = objects;
}