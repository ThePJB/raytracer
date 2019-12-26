#ifndef WRITE_IMAGE_H
#define WRITE_IMAGE_H

void write_image(int w, int h, unsigned char* data, char* filename);

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} pixel;

#endif