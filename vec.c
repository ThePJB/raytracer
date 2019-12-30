#include "OpenBLAS/cblas.h"

#include "vec.h"

// not sure if its actually faster using blas for a bunch of vec3 stuff but we will see. it probably unrolls and cooks it into vector instructions

inline double dot(vec3 x, vec3 y) {
    return cblas_ddot(3, (double *)&x, 1, (double *)&y, 1);
}

inline vec3 axpy(double a, vec3 x, vec3 y) {
    cblas_daxpy(3, a, (double *)&x, 1, (double *)&y, 1);
    return y;
}

inline vec3 sub(vec3 x, vec3 y) {
    cblas_daxpy(3, -1, (double *)&y, 1, (double *)&x, 1);
    return x;
}

inline vec3 make_unit(vec3 x) {
    cblas_dscal(3, 1/cblas_dnrm2(3, (double *)&x, 1), (double *)&x, 1);
    return x;
}

inline vec3 scale(double a, vec3 x) {
    cblas_dscal(3, a, (double *)&x, 1);
    return x;
}

inline vec3 proj(vec3 x, vec3 y) {
    double a = dot(x,y) / dot(x,x);
    return scale(a, x);
}

inline vec3 cross(vec3 x, vec3 y) {
    return (vec3) {
        .x = x.y * y.z - x.z * y.y,
        .y = x.z * y.x - x.x * y.z,
        .z = x.x * y.y - x.y * y.x,
    };
}

void vec_str(char* buf, vec3 a) {
    sprintf(buf, "[%f %f %f]", a.x, a.y, a.z);
}