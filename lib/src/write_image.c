#include "write_image.h"


#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define PNG_DEBUG 3
#include <png.h>

void abort_(const char * s, ...);

void write_image(int w, int h, unsigned char* data, char* filename) {
    png_byte color_type = PNG_COLOR_TYPE_RGB;
    png_byte bit_depth = 8;
    png_bytep row_pointers[h];
    for (int i = 0; i < h; i++) {
        row_pointers[i] = &data[i*w*3]; // magic *3 for rgb
    }
    FILE *fp = fopen(filename, "wb");
    if (!fp)
        abort_("[write_png_file] File %s could not be opened for writing", filename);
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
        abort_("[write_png_file] png_create_write_struct failed");
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        abort_("[write_png_file] png_create_info_struct failed");
    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during init_io");
    png_init_io(png_ptr, fp);
    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during writing header");
    png_set_IHDR(png_ptr, info_ptr, w, h,
        bit_depth, color_type, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(png_ptr, info_ptr);

    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during writing bytes");
    png_write_image(png_ptr, row_pointers); // this line

    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[write_png_file] Error during end of write");
    png_write_end(png_ptr, NULL);
 
    fclose(fp);
}




void abort_(const char * s, ...) {
    va_list args;
    va_start(args, s);
    vfprintf(stderr, s, args);
    fprintf(stderr, "\n");
    va_end(args);
    abort();
}
