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

#define M_PI 3.14159265358979323846
#define DEG_TO_RAD 0.01745329251994329577

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

    return normal_colour; // normal map

    if (scene->objects[closest].reflectance > 0) {
        ray reflected_ray = ray_reflect(r, closest_position, closest_normal);
        return cast_ray(scene, reflected_ray, depth+1);
    }
    return scene->objects[closest].colour;
}

void render_scene(vec3* image, scene* scene, int w, int h, camera cam) {

    // first relationship between angle and
    double wrange, hrange;

    wrange = 2 * tan(DEG_TO_RAD * cam.aov_horz / 2);
    hrange = 2 * tan(DEG_TO_RAD * cam.aov_vert / 2);

    double focal_length = 1;

    // angle relationship goes here
    vec3 view_plane_pos = axpy(focal_length, cam.dir, cam.pos);

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

    // what happesn to an axis vector: ok
    // what happens to a 45 degree guy? +x +z
    // i think this is legit

    // todo view angles

    u.x = cam.dir.z;
    u.z = -cam.dir.x;

    v.y = cam.dir.z;
    v.z = -cam.dir.y;

    int i = 0;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            vec3 uo = scale(wrange * (-0.5 + x/(double)w), u);
            vec3 vo = scale(hrange * (-0.5 + y/(double)h), v);

            vec3 v = view_plane_pos;
            v = axpy(1,v, uo);
            v = axpy(1,v, vo);

            vec3 dir = sub(v, cam.pos);
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

    camera straight = new_camera(0,0,-5,0,0,1,80,80);
    camera side = new_camera(5,0,0,-1,0,0,80,80);
    camera overhead = new_camera(0,-16,0,0,1,0,90,90);

    object objs[4];

    scene scene;
    //bigger_test(&scene);
    reflecto_test(&scene);

    render_scene(image, &scene, w,h, side);
    pixel pimage[w*h];

    for (int i = 0; i < w*h; i++) {
        pimage[i].r = (unsigned char) (255*image[i].x);
        pimage[i].g = (unsigned char) (255*image[i].y);
        pimage[i].b = (unsigned char) (255*image[i].z);
    }
    write_image(w,h,(unsigned char*)pimage, "out.png");
}

