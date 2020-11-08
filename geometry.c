#include "geometry.h"
#include "linmath.h"
#include <math.h>
#include <stdlib.h>

// ray as mat4 - not so sure about this but lets see
mat4 ray_as_mat4(ray r) {
    mat4 ret = {0};
    ret.asDoubleArray[0][0] = r.direction.x;
    ret.asDoubleArray[1][1] = r.direction.y;
    ret.asDoubleArray[2][2] = r.direction.z;

    ret.asDoubleArray[3][0] = r.origin.x;
    ret.asDoubleArray[3][1] = r.origin.y;
    ret.asDoubleArray[3][2] = r.origin.z;
    ret.asDoubleArray[3][3] = 1;

    return ret;
}

ray mat4_as_ray(mat4 a) {
    ray ret = {0};
    ret.direction.x = a.asDoubleArray[0][0];
    ret.direction.y = a.asDoubleArray[1][1];
    ret.direction.z = a.asDoubleArray[2][2];

    ret.direction = normalize(ret.direction);

    ret.origin.x = a.asDoubleArray[3][0];
    ret.origin.y = a.asDoubleArray[3][1];
    ret.origin.z = a.asDoubleArray[3][2];

    return ret;
}

ray transform_ray(mat4 transform, ray r) {
    vec3 p1 = r.origin;
    vec3 p2 = vec3_add(r.origin, r.direction);

    vec3 p1t = vec3_transform(transform, p1);
    vec3 p2t = vec3_transform(transform, p2);

    ray ret;
    ret.origin = p1t;
    ret.direction = normalize(vec3_sub(p2t, p1t));

    return ret;
}

intersection_info ray_intersects_tri(ray r, triangle o) {
    const double EPSILON = 0.0000000000001;
    /*
        Using Moller Trumbore intersection algorithm
        https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
    */

    intersection_info ret = {0};

    vec3 v0 = o.vert[0];
    vec3 v1 = o.vert[1];
    vec3 v2 = o.vert[2];
    
    vec3 e1 = vec3_sub(v1, v0);
    vec3 e2 = vec3_sub(v2, v0);

    vec3 h = cross(r.direction, e2); // aka pvec
    double a = dot(e1, h);           // aka det

    if (a > -EPSILON && a < EPSILON) {
        return ret;
    }

    double f = 1.0/a;       // aka invDet
        
    vec3 s = vec3_sub(r.origin, v0);    // aka tvec
    double u = f * dot(s, h);           // maybe this is barycentric or something
    if (u < 0 || u > 1) {
        return ret;
    }
    vec3 q = cross(s, e1);
    double v = dot(q, r.direction) * f;

    if (v < 0 || u+v > 1) {
        return ret;
    }

    double t = f * dot(e2, q);
    
    if (t < 0) {
        return ret;
    }

    ret.did_intersect = true;
    ret.distance = t;
    ret.position = vec3_add(r.origin, vec3_mul(r.direction, t));

    // adjust for backfacing triangle
    if (a > 0) {
        ret.normal = normalize(cross(e1,e2));
    } else {
        ret.normal = normalize(cross(e2,e1));
    }
    
    //ret.colour = o.colour;

    return ret;
}

// TODO check if its behind us
intersection_info ray_intersects_object(ray r, object o) {
    intersection_info ret = {0};

    if (o.tag == OBJ_NONE) {
        return ret;

    } else if (o.tag == OBJ_SPHERE) {
        vec3 L = vec3_sub(o.sphere.center, r.origin);
        double tca = dot(L, r.direction);
        if (tca < 0) return ret;

        double d2 = dot(L, L) - tca * tca;
        double r2 = o.sphere.radius * o.sphere.radius;
        if (d2 > r2) return ret;

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
                return ret;
            }
        }

        ret.did_intersect = true;
        ret.distance = t0;
        ret.position = vec3_add(vec3_mul(r.direction, t0), r.origin);
        ret.normal = normalize(vec3_sub(ret.position, o.sphere.center));
        ret.colour = o.colour;
        return ret;

    } else if (o.tag == OBJ_TRIANGLE) {
        intersection_info ii = ray_intersects_tri(r,o.triangle);
        ii.colour = o.colour;
        return ii;
    } else if (o.tag == OBJ_MESH) {
        intersection_info closest_ii = {0};
        closest_ii.distance = INFINITY;

        for (int i = 0; i < o.mesh.num_triangles; i++) {
            intersection_info ii = ray_intersects_tri(r, o.mesh.triangles[i]);
            if (ii.did_intersect && ii.distance < closest_ii.distance) {
                closest_ii = ii;
            }
        }

        closest_ii.colour = o.colour;
        return closest_ii;
    } else {
        printf("bad object tag\r\n");
        exit(1);
    }
}

void print_ray(ray r) {
    printf("Ray: origin [%.2f %.2f %.2f] dir [%.2f %.2f %.2f]\n", r.origin.x, r.origin.y, r.origin.z,
         r.direction.x, r.direction.y, r.direction.z);
}
