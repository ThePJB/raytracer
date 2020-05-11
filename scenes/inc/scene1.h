#ifndef SCENE1_H
#define SCENE1_H

#include "object.h"
#include "scene.h"

scene make_scene1() {
    return (scene) {
        .objects = {
            {
                .type = SPHERE,
                .colour = {1.0, 0.0, 0.0},
                .reflectance = 0.0,
                (sphere) {
                    .center = {10, 0, 0},
                    .radius = 4,
                }
            },            
            {
                .type = SPHERE,
                .colour = {0.0, 1.0, 0.0},
                .reflectance = 0.0,
                (sphere) {
                    .center = {10, 1, 0},
                    .radius = 4,
                }
            },            
            {
                .type = SPHERE,
                .colour = {0.0, 0.0, 1.0},
                .reflectance = 0.0,
                (sphere) {
                    .center = {8, 0.5, 0},
                    .radius = 3,
                }
            },
        },
        .cameras = {
            make_camera(
                (vec3) {0, 0, 0},
                (vec3) {1, 0, 0},
                (vec3) {0, 0, 1},
                90, 90)
        },
        .bg_colour = {0.5, 0, 0.5}
    };
}

#endif