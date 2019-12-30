#include "camera.h"
#include <math.h>
#include <stdio.h>

#define M_PI 3.14159265358979323846
#define DEG_TO_RAD 0.01745329251994329577

void make_camera(camera* camera, vec3 pos, vec3 vpn, vec3 vpu, double av, double ah) {
    camera->pos = pos;
    camera->vpn = vpn;
    camera->vpu = vpu;
    camera->vpr = cross(vpu, vpn);
    camera->aov_vert = av;
    camera->aov_horz = ah;
    camera->wrange = 2 * tan(DEG_TO_RAD * ah / 2);
    camera->hrange = 2 * tan(DEG_TO_RAD * av / 2);
}

ray get_pixel_ray(camera camera, int x, int xmax, int y, int ymax) {
    ray ray;
    ray.origin = camera.pos;

    double wrange, hrange;

    double focal_length = 1;

    double camup_coeff = camera.hrange * (-0.5 + y/(double)ymax);
    double camright_coeff = camera.wrange * (-0.5 + x/(double)xmax);

    vec3 target = axpy(focal_length, camera.vpn, camera.pos);
    target = axpy(camup_coeff, camera.vpu, target);
    target = axpy(camright_coeff, camera.vpr, target);

    ray.direction = make_unit(sub(target, ray.origin));

    return ray;
}

void test_camera() {
    camera straight;
    make_camera(&straight, (vec3) {0, 0, -5},
        (vec3) {0, 0, 1},
        (vec3) {0, -1, 0},
        80, 80);

    char buf[128];
    ray_str(buf, get_pixel_ray(straight, 0, 100, 0, 100));
    printf("0,0: %s\n", buf);
    ray_str(buf, get_pixel_ray(straight, 100, 100, 0, 100));
    printf("100,0: %s\n", buf);
    ray_str(buf, get_pixel_ray(straight, 0, 100, 100, 100));
    printf("0,100: %s\n", buf);
    ray_str(buf, get_pixel_ray(straight, 100, 100, 100, 100));
    printf("100,100: %s\n", buf);
    ray_str(buf, get_pixel_ray(straight, 50, 100, 50, 100));
    printf("50,50: %s\n", buf);
    ray_str(buf, get_pixel_ray(straight, 0, 100, 50, 100));
    printf("0,50: %s\n", buf);
    ray_str(buf, get_pixel_ray(straight, 50, 100, 0, 100));
    printf("50,0: %s\n", buf);

}