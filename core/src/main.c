#include <stdio.h>
#include "png.h"
#include "write_image.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "object.h"
#include "vec.h"
#include "scene1.h"
#include "camera.h"

#include <stdbool.h>

#define PNG_DEBUG 3
#include <png.h>

#define M_PI 3.14159265358979323846
#define DEG_TO_RAD 0.01745329251994329577

// bugs:
// reflective plane always reflects itself?
// reflective plane weird reflections on the horizon
// from directly above normal plane is cooked

// view plane adds x and y but it should actually be adding orthogonal vectors

// swap i think that its hard to have rules because you can say swap z and y, but what if you just orient camera another day.
// maybe need to just do it another way like come up with transformation matrix

// features:
// multiple cameras
// generate camera looking at something



// todo maybe roll


const int MAX_DEPTH = 15;

// returns the colour it sees
vec3 cast_ray(scene* scene, ray r, int depth) {
    if (depth > MAX_DEPTH) {
        return (vec3) {1, 1, 1};
    }

    int closest = -1;
    double closest_dist = 1.0/0.0;
    vec3 closest_normal;
    vec3 closest_position;

    
    int i = 0;
    object obj;
    while ((obj = scene->objects[i]).type != NONE) {
        double distance;
        vec3 normal;
        vec3 position;
        if (ray_intersects_object(&normal, &position, &distance, r, obj)) {
            if (distance < closest_dist) {
                closest = i;
                closest_dist = distance;
                closest_normal = normal;
                closest_position = position;
            }
        }
        i++;
    }

    if (closest == -1) {
        return scene->bg_colour;
    }
    vec3 normal_colour =  scale(0.5,axpy(1, closest_normal, (vec3) {1,1,1}));

    return normal_colour; // normal map

    if (obj.reflectance > 0) {
        ray reflected_ray = ray_reflect(r, closest_position, closest_normal);
        return cast_ray(scene, reflected_ray, depth+1);
    }
    return scene->objects[closest].colour;
}

void render_scene(vec3* image, scene* scene, int w, int h, int cam) {

    int i = 0;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {

            ray ray = get_pixel_ray(scene->cameras[cam], x, w, y, h);

            image[i] = cast_ray(scene, ray, 0);

            i += 1;
        }
    }
}

int main(int argc, char **argv) {
    int w = 250;
    int h = 250;
    // program bails if I increase this any further, VLA maximum??

    vec3 image[w*h];

    camera straight, side, overhead;

    scene scene1 = make_scene1();

    render_scene(image, &scene1, w,h, 0);
    pixel pimage[w*h];

    for (int i = 0; i < w*h; i++) {
        pimage[i].r = (unsigned char) (255*image[i].x);
        pimage[i].g = (unsigned char) (255*image[i].y);
        pimage[i].b = (unsigned char) (255*image[i].z);
    }
    write_image(w,h,(unsigned char*)pimage, "out.png");
}

