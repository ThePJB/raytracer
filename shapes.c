#include "shapes.h"
#include "vec.h"
#include "math.h"
#include <stdio.h>

// True for intersection, will populate normal and position
int ray_intersects_object(vec3 *normal, vec3 *position, double *dist, ray ray, object object) {
    vec3 L;
    double tca;

    double denom;

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

        case PLANE:
            denom = dot(object.plane.normal, ray.direction);
            if (denom > 0) {
            //if (denom > 1e-6) {
                vec3 p0l0 = sub(object.plane.center, ray.origin);
                double t = dot(p0l0, object.plane.normal) / denom;
                *dist = t;
                *position = p0l0;
                *normal = object.plane.normal;
                return (t >= 0);

                // so a square would have the same but you would clamp x and y of p-p0l0
            }

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
    char buf1[64];
    char buf2[64];
    char buf3[64];

    switch (object.type) {
        case SPHERE:
            vec_str(buf1, object.sphere.center);
            vec_str(buf2, object.colour);
            sprintf(buf, "Sphere centered at %s, radius %f, colour %s", buf1, object.sphere.radius, buf2);
            break;
        case PLANE:
            vec_str(buf1, object.plane.center);
            vec_str(buf2, object.plane.normal);
            vec_str(buf3, object.colour);
            sprintf(buf, "Plane centered at %s with normal %s, colour %s", buf1, buf2, buf3);
            break;
    }
}

