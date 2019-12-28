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
#include "scenes.h"

#include <stdbool.h>

#define PNG_DEBUG 3
#include <png.h>


// bugs:
// reflective plane always reflects itself?
// reflective plane weird reflections on the horizon
// from directly above normal plane is cooked

// view plane adds x and y but it should actually be adding orthogonal vectors

// features:
// multiple cameras
// generate camera looking at something



typedef struct {
    vec3 pos;
    vec3 dir;
    double aov_horz;
    double aov_vert;
} camera;

// todo maybe roll


camera new_camera(double x,double y,double z, double u, double v, double w, double ah, double av) {
    return (camera) {
        .pos = (vec3) {.x = x, .y = y, .z = z},
        .dir = make_unit((vec3) {.x = u, .y = v, .z = w}),
        .aov_horz = ah,
        .aov_vert = av,
    };
}


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

    for (int i = 0; i < scene->numObjects; i++) {
        double distance;
        vec3 normal;
        vec3 position;
        if (ray_intersects_object(&normal, &position, &distance, r, scene->objects[i])) {
            if (distance < closest_dist) {
                closest = i;
                closest_dist = distance;
                closest_normal = normal;
                closest_position = position;
            }
        }
    }

    if (closest == -1) {
        return scene->bg_colour;
    }
    vec3 normal_colour =  scale(0.5,axpy(1, closest_normal, (vec3) {1,1,1}));

    //return normal_colour; // normal map

    if (scene->objects[closest].reflectance > 0) {
        ray reflected_ray = ray_reflect(r, closest_position, closest_normal);
        return cast_ray(scene, reflected_ray, depth+1);
    }
    return scene->objects[closest].colour;
}

void render_scene(vec3* image, scene* scene, int w, int h, camera cam) {

    // first relationship between angle and
    

    vec3 view_plane_pos = axpy(1, cam.dir, cam.pos);

    // now get direction from camera_pos to all of the imaginary pixels on the view plane
    char campos[128];
    char camdir[128];
    char sstr[128];

    vec_str(campos, cam.pos);
    vec_str(camdir, cam.dir);

    object_str(sstr, scene->objects[0]);

    //printf("Rendering scene, campos %s, camdir %s, focal len %f, contains sphere %s\n", campos, camdir, focal_length, sstr);
    
    // this is kinda 1 pixel offset but you wouldnt really be able to tell at higher resolutions lol
    
    // find these with gram schmidt??
    // or do some cross product shit?
    // transformation required to get that direction vector, then apply it to x and y as well
    vec3 u,v;



    int i = 0;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {

            double xo = -0.5 + ((double)x/(double)w);
            double yo = -0.5 + ((double)y/(double)h);

            vec3 v = view_plane_pos;
            v.x += xo;
            v.y += yo;
            
            vec3 dir = sub(v,cam.pos);

            dir = make_unit(dir);

            ray ray = {.origin = cam.pos, .direction = dir};
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

    camera straight = new_camera(0,0,-5,0,0,1,0.5);
    camera overhead = new_camera(0,-16,0,0,1,0,0.5);

    object objs[4];

    scene scene;
    //bigger_test(&scene);
    reflecto_test(&scene);

    render_scene(image, &scene, w,h, overhead);
    pixel pimage[w*h];

    for (int i = 0; i < w*h; i++) {
        pimage[i].r = (unsigned char) (255*image[i].x);
        pimage[i].g = (unsigned char) (255*image[i].y);
        pimage[i].b = (unsigned char) (255*image[i].z);
    }
    write_image(w,h,(unsigned char*)pimage, "out.png");
}

