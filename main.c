#include <stdio.h>
#include "png.h"
#include "write_image.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "vec.h"

#include <stdbool.h>

#define PNG_DEBUG 3
#include <png.h>



typedef struct {
    float r,g,b;
} colour;

// Objects
// Ray intersects object, where, normal

typedef enum {
    SPHERE,
    NUM_OBJECT_TYPES,
} object_type;

typedef struct {
    vec3 center;
    double radius;
    colour colour;
} sphere;

typedef struct {
    object_type type;
    union {
        sphere sphere;
    };

} object;



typedef struct {
    vec3 origin, direction;
} ray;

typedef struct {
    object* objects;
    int numObjects;
} scene;

const int MAX_DEPTH = 15;

// True for intersection, will populate normal and position
int ray_intersects_object(vec3 *normal, vec3 *position, ray ray, object object) {
    switch (object.type) {
        case SPHERE:


        break;
    }
}

colour cast_ray(scene* scene, ray r, int depth) {
    return (colour) {1,0,0};
}

void render_scene(colour* image, scene* scene, int w, int h, vec3 camera_pos, vec3 camera_dir, double focal_length) {
    vec3 view_plane_pos = axpy(focal_length, camera_dir, camera_pos);

    // now get direction from camera_pos to all of the imaginary pixels on the view plane

    int i = 0;
    //for (double x = -1; x < 1; x += 1/(double)w) {
    for (int x = 0; x < w; x++) {
        //for (double y = -1; y < 1; y += 1/(double)h) {
        for (int y = 0; y < h; y++) {

            double xo = -0.5 * ((double)x/(double)w);
            double yo = -0.5 * ((double)y/(double)h);

            vec3 v = view_plane_pos;
            v.x += xo;
            v.y += yo;

            vec3 dir = sub(v, view_plane_pos);
            image[i] = cast_ray(scene, (ray){.origin = camera_pos, .direction = dir}, 0);

            i += 1;
        }
    }
    
}



int main(int argc, char **argv) {
    int w = 10;
    int h = 3;
    colour image[w*h];

    double focal_length = 2;
    vec3 cam_pos = {0, 0, -1*focal_length};
    vec3 cam_dir = {0,0,1};

    scene scene = {0, 0};
    render_scene(image, &scene, w,h,cam_pos,cam_dir, focal_length);
    pixel pimage[w*h];

    for (int i = 0; i < w*h; i++) {
        printf("[%f %f %f]", image[i].r, image[i].g, image[i].b);
        pimage[i].r = (unsigned char) (255*image[i].r);
        pimage[i].g = (unsigned char) (255*image[i].g);
        pimage[i].b = (unsigned char) (255*image[i].b);
        printf("[%d %d %d]", pimage[i].r, pimage[i].g, pimage[i].b);

    }
    write_image(w,h,(unsigned char*)pimage, "out.png");
}

