#include <stdio.h>
#include "png.h"
#include "write_image.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "shapes.h"
#include "vec.h"

#include <stdbool.h>

#define PNG_DEBUG 3
#include <png.h>


// Objects
// Ray intersects object, where, normal

typedef struct {
    object* objects;
    int numObjects;
    vec3 bg_colour;
} scene;

const int MAX_DEPTH = 15;

// returns the colour it sees
vec3 cast_ray(scene* scene, ray r, int depth) {
    int closest = -1;
    double closest_dist = 1.0/0.0;
    for (int i = 0; i < scene->numObjects; i++) {
        double distance;
        vec3 normal;
        vec3 position;
        if (ray_intersects_object(&normal, &position, &distance, r, scene->objects[i])) {
            if (distance < closest_dist) {
                closest = i;
                closest_dist = distance;
            }
        }
    }

    if (closest == -1) {
        return scene->bg_colour;
    }
    return scene->objects[closest].colour;
}

void render_scene(vec3* image, scene* scene, int w, int h, vec3 camera_pos, vec3 camera_dir, double focal_length) {
    vec3 view_plane_pos = axpy(focal_length, camera_dir, camera_pos);

    // now get direction from camera_pos to all of the imaginary pixels on the view plane
    char campos[128];
    char camdir[128];
    char sstr[128];

    vec_str(campos, camera_pos);
    vec_str(camdir, camera_dir);

    object_str(sstr, scene->objects[0]);

    printf("Rendering scene, campos %s, camdir %s, focal len %f, contains sphere %s\n", campos, camdir, focal_length, sstr);
    
    // this is kinda 1 pixel offset but you wouldnt really be able to tell at higher resolutions lol
    int i = 0;
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {

            double xo = -0.5 + ((double)x/(double)w);
            double yo = -0.5 + ((double)y/(double)h);

            vec3 v = view_plane_pos;
            v.x += xo;
            v.y += yo;
            
            vec3 dir = sub(v,camera_pos);

            dir = make_unit(dir);

            ray ray = {.origin = camera_pos, .direction = dir};
            image[i] = cast_ray(scene, ray, 0);
            
            char buf[128];
            char vbuf[128];
            ray_str(buf, ray);
            vec_str(vbuf, image[i]);

            //printf("cast ray %s, found colour %s\n", buf, vbuf);        

            i += 1;
        }
    }
}

int main(int argc, char **argv) {
    int w = 250;
    int h = 250;
    // program bails if I increase this any further, VLA maximum??

    vec3 image[w*h];

    double focal_length = 2;
    vec3 cam_pos = {0, 0, -1*focal_length};
    vec3 cam_dir = {0,0,1};

    object objs[4];

    objs[0] = (object) {
        .colour = (vec3) {0.0, 1.0, 0.0},
        .type = SPHERE,
        .sphere = (sphere) {
            .center = (vec3) {0,0,5.0},
            .radius = 1,
        }
    };

    objs[1] = (object) {
        .colour = (vec3) {0.7, 0.7, 0.2},
        .type = SPHERE,
        .sphere = (sphere) {
            .center = (vec3) {-1,0.2,5.4},
            .radius = 1.1,
        }
    };

    objs[2] = (object) {
        .colour = (vec3) {0.2, 0.2, 0.2},
        .type = SPHERE,
        .sphere = (sphere) {
            .center = (vec3) {1,0.7,4.7},
            .radius = 0.9,
        }
    };

    objs[3] = (object) {
        .colour = (vec3) {1, 0.2, 0.2},
        .type = SPHERE,
        .sphere = (sphere) {
            .center = (vec3) {0.9,-0.2,4.7},
            .radius = 0.5,
        }
    };

    scene scene = {
        .numObjects = 4,
        .objects =  objs,
        .bg_colour = (vec3) {0.5, 0.0, 0.5}};
    render_scene(image, &scene, w,h,cam_pos,cam_dir, focal_length);
    pixel pimage[w*h];

    for (int i = 0; i < w*h; i++) {
        pimage[i].r = (unsigned char) (255*image[i].x);
        pimage[i].g = (unsigned char) (255*image[i].y);
        pimage[i].b = (unsigned char) (255*image[i].z);
    }
    write_image(w,h,(unsigned char*)pimage, "out.png");
}

