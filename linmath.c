#include <math.h>
#include <stdio.h>
#include "linmath.h"


#define DEG_TO_RAD 0.01745329251994329577

vec3 new_vec3(double x, double y, double z) {
    return (vec3) {x, y, z};
}

vec3 vec3_add(vec3 a, vec3 b) {
    return new_vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

vec3 vec3_sub(vec3 a, vec3 b) {
    return new_vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

vec3 vec3_mul(vec3 a, double j) {
    return new_vec3(a.x*j, a.y*j, a.z*j);
}

vec3 vec3_mul_vec3 (vec3 a, vec3 b) {
    return (vec3) {a.x*b.x, a.y*b.y, a.z*b.z};
}


vec3 vec3_div(vec3 a, double j) {
    return new_vec3(a.x/j, a.y/j, a.z/j);
}

vec3 vec3_lerp(vec3 a, vec3 b, double j) {
    return vec3_add(vec3_mul(a, j), vec3_mul(b, 1-j));
}

vec4 vec4_div(vec4 a, double j) {
    return (vec4) {a.x/j, a.y/j, a.z/j, a.w/j};
}

vec3 normalize(vec3 a) {
    return vec3_div(a, vec3_abs(a));
}

double dot(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

double vec3_abs(vec3 a) {
    return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

vec3 cross(vec3 a, vec3 b) {
    return new_vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - b.x*a.y);
}

vec3 normal_to_colour(vec3 a) {
    return new_vec3(
        a.x / 2 + 0.5,
        a.y / 2 + 0.5,
        a.z / 2 + 0.5
    );
}

mat3 rx(double theta) {
    return (mat3) {
        1, 0, 0,
        0, cos(theta), -sin(theta),
        0, sin(theta), cos(theta),
    };
}
mat3 ry(double theta) {
    return (mat3) {
        cos(theta), 0, sin(theta),
        0, 1, 0,
        -sin(theta), 0, cos(theta),
    };
}
mat3 rz(double theta) {
    return (mat3) {
        cos(theta), -sin(theta), 0,
        sin(theta), cos(theta), -0,
        0, 0, 1,
    };
}

mat3 mat3_mul_mat3(mat3 a, mat3 b) {
    mat3 ret = {0};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            double acc = 0;
            for (int k = 0; k < 3; k++) {
                // row dot column
                acc += a.asDoubleArray[i][k] * b.asDoubleArray[k][j];
            }
            ret.asDoubleArray[i][j] = acc;
        }
    }
    return ret;
}

mat4 pad_to_mat4(mat3 a) {
    double *ad = a.asDoubleArray;
    return (mat4) {
        ad[0], ad[1], ad[2], 0,
        ad[3], ad[4], ad[5], 0,
        ad[6], ad[7], ad[8], 0,
        0, 0, 0, 1,  
    };
}

mat4 transformation_mat(double x, double y, double z, double roll, double pitch, double yaw, double scale) {
    mat3 rot_mat = mat3_mul_mat3(rz(DEG_TO_RAD * yaw), mat3_mul_mat3(ry(DEG_TO_RAD * pitch), rx(DEG_TO_RAD * roll)));
    mat3 scale_mat = (mat3) {
        scale, 0, 0,
        0, scale, 0,
        0, 0, scale,
    };

    mat4 translation_mat = (mat4) {
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1,
    };

    return mat4_mul(translation_mat, mat4_mul(pad_to_mat4(rot_mat), pad_to_mat4(scale_mat)));
}

mat4 mat4_mul(mat4 a, mat4 b) {
    mat4 ret = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            double acc = 0;
            for (int k = 0; k < 4; k++) {
                // row dot column
                acc += a.asDoubleArray[i][k] * b.asDoubleArray[k][j];
            }
            ret.asDoubleArray[i][j] = acc;
        }
    }
    return ret;
}

mat4 cam_mat_lookat(vec3 pos, vec3 up, vec3 target) {
    vec3 look = normalize(vec3_sub(pos, target));
    vec3 right = normalize(cross(up, look));
    up = cross(look, right);
    
    // transposed = inverse (its orthonormal)
    mat4 orientation = {
        right.x, up.x, look.x, 0,
        right.y, up.y, look.y, 0,
        right.z, up.z, look.z, 0,
        0, 0, 0, 1
    };

    // negated = inverse for this guy
    mat4 translation = {
        1, 0, 0, pos.x,
        0, 1, 0, pos.y,
        0, 0, 1, pos.z,
        0, 0, 0, 1,
    };

    return mat4_mul(translation, orientation);
}

// inverse of cam mat
mat4 view_mat_lookat(vec3 pos, vec3 up, vec3 target) {
    vec3 look = normalize(vec3_sub(target, pos));
    vec3 right = normalize(cross(up, look));
    up = cross(look, right);
    
    // transposed = inverse (its orthonormal)
    mat4 orientation = {
        look.x, right.x, up.x, 0,
        look.y, right.y, up.y, 0,
        look.z, right.z, up.z, 0,
        0, 0, 0, 1
    };

    printf("camera orientation:\n");
    print_mat4(orientation);

    // negated = inverse for this guy
    mat4 translation = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -pos.x, -pos.y, -pos.z, 1,
    };
    printf("camera pos:\n");
    print_mat4(translation);

    // order is reversed because they are inverted
    return mat4_mul(orientation, translation);
}

mat4 make_perspective_mat(double fovx, double fovy, double znear, double zfar) {
    return (mat4) {
        atan2(fovx, 2), 0, 0, 0,
        0, atan2(fovy, 2), 0, 0,
        0, 0, -(zfar + znear)/(zfar-znear), -2*(znear*zfar)/(zfar-znear),
        0, 0, -1, 0
    };
}

mat4 inverse_perspective_mat(double fovx, double fovy, double znear, double zfar) {
    double a = atan2(fovx, 2);
    double b = atan2(fovy, 2);
    double c = -(zfar + znear)/(zfar-znear);
    double d = -2*(znear*zfar)/(zfar-znear);
    double e = -1;

    // -c/de = - (znear + zfar)/znear*zfar   i think

    return (mat4) {
        1/a, 0, 0, 0,
        0, 1/b, 0, 0,
        0, 0, 0, 1/e,
        0, 0, 1/d, -c / d * e, 
    };
}

void print_mat4(mat4 a) {
    for (int i = 0; i < 4; i++) {
        printf("\t|");
        for (int j = 0; j < 4; j++) {
            printf("%.1f", a.asDoubleArray[i][j]);
            if (j == 3) {
                printf("|");
            } else {
                printf(", ");
            }
        }
        printf("\n");
    }
}

mat4 mat4_transpose(mat4 a) {
    mat4 ret = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ret.asDoubleArray[i][j] = a.asDoubleArray[j][i];
        }
    }
    return ret;
}

vec4 vec4_mul_mat4(mat4 t, vec4 a) {
    vec4 ret = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            double acc = 0;
            // row dot column
            ret.asArray[i] += t.asDoubleArray[i][j] * a.asArray[j];
        }
    }
    return ret;
}

// homogeneous transformation
vec3 vec3_transform(mat4 transform, vec3 a) {
    // so: virtually pad the vector with a 1
    // perform mul (which way)
    vec4 ah = (vec4) {a.x, a.y, a.z, 1};

    ah = vec4_mul_mat4(transform, ah);
    ah = vec4_div(ah, ah.w);

    return (vec3) {ah.x, ah.y, ah.z};

    // normalize it again by dividing by w
}