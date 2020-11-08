#include "linmath.h"
#include "scene.h"
#include "geometry.h"
#include "write_image.h"

#include <stdlib.h>

#define M_PI 3.14159265358979323846
#define DEG_TO_RAD 0.01745329251994329577
#define MAX_DEPTH 15
#define SHADOW_BIAS 0.01

bool cast_shadow_ray(scene *s, vec3 P, vec3 L);
vec3 cast_ray(scene *s, ray r, int depth, shading_mode shading_mode);

double max(double a, double b) {
    return a > b ? a : b;
}
double min(double a, double b) {
    return a < b ? a : b;
}

// render scene
void render_scene(scene *s, char *filename, int w, int h, int camera_index,
     double fovx, double a, shading_mode shading_mode) {

    printf("Rendering %s. Using camera:\n", filename);
    print_mat4(s->cameras[camera_index]);

    vec3 *buf = calloc(w*h, sizeof(vec3));
    pixel *pixbuf = calloc(w*h, sizeof(pixel));

    double fovy = 1 / (a / fovx);

    int last_percent = 0;

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            
            // Keep track of progress
            int current_percent = 100 * (x*h + y) / (w * h);
            if (current_percent > last_percent) {
                last_percent = current_percent;
                printf("%d%% complete\n", current_percent);
            }
            

            // move it to coordinates -1 to 1
            double normalized_x = (double)(x+0.5)*2 / (double)w - 1;
            double normalized_y = (double)(y+0.5)*2 / (double)h - 1;
            normalized_y *= -1; // screen coords go the other way

            // inverse perspective transform
            double d = 1; //  projection plane

            double u = normalized_x * a * tan(DEG_TO_RAD * fovx/2);
            double v = normalized_y * tan(DEG_TO_RAD * fovx/2);

            vec3 proj_plane_intersection = {u, v, -d};

            ray r = {
                .origin = {0, 0, 0},
                .direction = normalize(proj_plane_intersection),
            };

            // camera transform (inverse view)
            r = transform_ray(s->cameras[camera_index], r);

            buf[y*w + x] = cast_ray(s, r, MAX_DEPTH, shading_mode);
        }
    }

    for (int i = 0; i < w*h; i++) {
        pixbuf[i].r = (unsigned char)(buf[i].x * 255);
        pixbuf[i].g = (unsigned char)(buf[i].y * 255);
        pixbuf[i].b = (unsigned char)(buf[i].z * 255);
    }

    write_image(w, h, (unsigned char *)pixbuf, filename);

    free(buf);
    free(pixbuf);
}


// returns colour
vec3 cast_ray(scene *s, ray r, int depth, shading_mode shading_mode) {
    intersection_info closest_ii = {0};
    closest_ii.distance = INFINITY;

    for (int i = 0; i < MAX_OBJECTS; i++) {
        object o = s->objects[i];
        if (o.tag == OBJ_NONE) {
            break;
        }

        // get nearest intersection
        intersection_info ii = ray_intersects_object(r, o);
        if (ii.did_intersect && ii.distance < closest_ii.distance) {
            closest_ii = ii;
        }
    }

    // for gooch and facing ratio
    vec3 light_dir = normalize((vec3){1.1, 1.2, 1.3});

    if (shading_mode == SHADING_AMBIENT) {
        return closest_ii.colour;

    } else if (shading_mode == SHADING_FACING_RATIO) {
        if (!closest_ii.did_intersect) return (vec3){0,0,0};

        double dp = dot(closest_ii.normal, light_dir);
        if (dp < 0) {
            dp = 0;
        }
        return vec3_mul(closest_ii.colour, dp);

    } else if (shading_mode == SHADING_NORMALS) {
        if (!closest_ii.did_intersect) return (vec3){0,0,0};

        return normal_to_colour(closest_ii.normal);

    } else if (shading_mode == SHADING_GOOCH) {
        if (!closest_ii.did_intersect) return (vec3){0,0,0};

        vec3 gooch_warm = (vec3) {1,1,0};
        vec3 gooch_mid = (vec3) {0.5, 0.5, 0.5};
        vec3 gooch_cool = (vec3) {0,0,1};

        // this will be like facing ratio but if its negative, abs times cool
        double dp = dot(closest_ii.normal, light_dir);
        if (dp > 0) {
            return vec3_lerp(gooch_warm, gooch_mid, dp);
        } else {
            return vec3_lerp(gooch_cool, gooch_mid, -1*dp);
        }

    } else if (shading_mode == SHADING_LIGHTS) {
        if (!closest_ii.did_intersect) return (vec3){0,0,0};
        vec3 acc = {0};

        for (int i = 0; i < MAX_LIGHTS; i++) {
            point_light pl = s->lights[i];
            if (pl.intensity == 0) {
                continue;
            }
            vec3 L = vec3_sub(pl.pos, closest_ii.position);
            double d = vec3_abs(L);

            vec3 Pprime = closest_ii.position;// vec3_add(closest_ii.position, vec3_mul(closest_ii.normal, SHADOW_BIAS));

            if (cast_shadow_ray(s, Pprime, L)) {
                //printf("sees the light\n");
                // accumulate lighting from this light
                //double intensity = (1 / d*d) * pl.intensity; // inverse square law, lets see
                //double intensity = 1.0/(d*d) * pl.intensity;
                double intensity = 1.0/(d) * pl.intensity;
                //double intensity = 1;
                double NdotL = max(0, dot(closest_ii.normal, normalize(L)));
                /*
                // debug colouring for negative dot product
                if (NdotL == 0) {
                    acc = vec3_add(acc, (vec3) {0, 0.5, 0});
                    continue;
                }
                */
                
                vec3 contribution = vec3_mul_vec3(vec3_div(pl.colour, M_PI), vec3_mul(closest_ii.colour, intensity * NdotL));
                
                acc = vec3_add(acc, contribution);
            }
        }
        acc.x = min(acc.x, 1);
        acc.y = min(acc.y, 1);
        acc.z = min(acc.z, 1);
        return acc;
        //return (vec3){1,0,1};
    } else {
            printf("Invalid shading mode\n");
            return (vec3) {1,0,1};
    }
}

// Seeing if there is any geometry occluding the light
bool cast_shadow_ray(scene *s, vec3 P, vec3 L) {
    ray r = {
        .origin = P,
        .direction = normalize(L),
    };

    double light_dist = vec3_abs(L);

    intersection_info closest_ii = {0};
    closest_ii.distance = INFINITY;

    for (int i = 0; i < MAX_OBJECTS; i++) {
        object o = s->objects[i];
        if (o.tag == OBJ_NONE) {
            break;
        }

        // get nearest intersection
        intersection_info ii = ray_intersects_object(r, o);
        if (ii.did_intersect && ii.distance < closest_ii.distance) {
            closest_ii = ii;
        }
    }

    return (closest_ii.distance > light_dist);
}