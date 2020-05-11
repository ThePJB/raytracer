#ifndef CAMERA_H
#define CAMERA_H

#include "vec.h"
#include "object.h"

typedef struct {
    vec3 pos;
    vec3 vpn;
    vec3 vpu;
    vec3 vpr; // or l?
    double aov_horz;
    double aov_vert;
    double wrange;
    double hrange;
} camera;

ray get_pixel_ray(camera camera, int x, int xmax, int y, int ymax);
camera make_camera(vec3 pos, vec3 vpn, vec3 vpu, double av, double ah);

//void make_camera(camera* camera, vec3 pos, vec3 vpn, vec3 vpu, double av, double ah); // fill in vpl with cross product
//void look_at_camera(camera* camera, vec3 pos, vec3 target, double roll, double av, double ah); // this is an example of how you want to compute vpu automatically

//void test_camera();

#endif