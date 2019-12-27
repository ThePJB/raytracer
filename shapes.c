#include "shapes.h"
#include "vec.h"
#include "math.h"
#include <stdio.h>

// True for intersection, will populate normal and position
int ray_intersects_object(vec3 *normal, vec3 *position, double *dist, ray ray, object object) {
    vec3 L;
    double tca;

    switch (object.type) {

        case SPHERE:
            L = sub(object.sphere.center, ray.origin);
            tca = dot(L, ray.direction);
            if (tca < 0) return 0;
            double d2 = dot(L, L) - tca * tca;
            double r2 = object.sphere.radius*object.sphere.radius;
            if (d2 > r2) return 0;
            double thc = sqrt(r2-d2);
            double t0 = tca - thc;
            double t1 = tca + thc;
            
            if (t0 > t1) {
                double tmp = t1;
                t1 = t0;
                t0 = tmp;
            }

            if (t0 < 0) {
                t0 = t1;
                if (t0 < 0) {
                    return 0;
                }
            }

            *dist = t0;
            *position = axpy(t0, ray.direction, ray.origin);
            *normal = make_unit(sub(*position, object.sphere.center));
            //*normal = todo
            // o yea chuck it out if its behind us as well, todo
            return 1;

        break;
    }
}

ray ray_reflect(ray incident, vec3 location, vec3 normal) {
    // reflect a dir about a normal?
    vec3 reflected_dir = sub(incident.direction, scale(2 * dot(incident.direction, normal), normal));
    return (ray) {.origin = location, .direction = reflected_dir};
}

void ray_str(char* buf, ray ray) {
    char v1buf[64];
    char v2buf[64];
    vec_str(v1buf, ray.origin);
    vec_str(v2buf, ray.direction);
    sprintf(buf, "orig: %s, dir: %s", v1buf, v2buf);
}

void object_str(char* buf, object object) {
    char vsbuf[64];
    char cbuf[64];

    switch (object.type) {
        case SPHERE:
            vec_str(vsbuf, object.sphere.center);
            vec_str(cbuf, object.colour);
            sprintf(buf, "Sphere centered at %s, radius %f, colour %s", vsbuf, object.sphere.radius, cbuf);
    }
}

