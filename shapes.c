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
            
            *dist = t0;
            *position = axpy(t0, ray.direction, ray.origin);
            //*normal = todo
            // o yea chuck it out if its behind us as well, todo
            return 1;

        break;
    }
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