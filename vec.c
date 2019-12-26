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

void print_vec(vec3 a) {
    printf("[%f %f %f]", a.x, a.y, a.z);
}