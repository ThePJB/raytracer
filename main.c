#include <stdio.h>
#include "OpenBLAS/cblas.h"
#include "png.h"
#include "write_image.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define PNG_DEBUG 3
#include <png.h>

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} pixel;

int main(int argc, char **argv) {

    pixel image[16] = {0};

    image[10].r=255;
    image[11] = (pixel) {255, 255, 255};
    image[14] = (pixel) {.r = 128, .g = 255, .b = 0};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("[%d %d %d]", image[i+j*4].r, image[i+j*4].g, image[i+j*4].b);
        }
        printf("\n");
    }

    write_image(4,4,(unsigned char*)image, "out.png");

    return 0;
}

