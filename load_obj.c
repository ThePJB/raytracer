#include "geometry.h"
#include <stdlib.h>

#include <float.h>
#include <limits.h>
#include <math.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define LOADER_MAX_LINE_LENGTH 100

// we are just praying that tokbuf is long enough
bool tok(const char* orig, char *tokbuf, char sep, int *pos, int bufsize) {
    int i = 0;
    char c;
    memset(tokbuf, '\0', bufsize);
    while ((c = orig[(*pos)++]) != '\0') {
        if (c == sep) {
            if (i == 0) {
                continue; // skip if it starts with a sep
            }
            return true;
        }
        tokbuf[i++] = c;
    }
    if (i != 0) {
        return true;
    } else {
        *pos = 0; // not sure if it should be our responsibility but it does make callers life a bit easier
        return false;
    }
}

mesh load_obj(char * path) {
    printf("loading file %s...", path);

    // slurp file
    char *buffer = 0;
    long length;
    FILE * f = fopen (path, "rb");
    mesh ret;
    int nv = 0;
    int nf = 0;

    if (f) {
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = calloc(length + 1, 1);
        if (buffer) {
            fread (buffer, 1, length, f);
        }
        fclose (f);
    }

    if (!buffer) {
        printf("problem loading file %s\n", path);
        exit(1);
    }

    // count number of v's and allocate vertex buffer
    for (int i = 0; i < length; i++) {
        if (buffer[i] == 'v') {
            nv++;
        } else if (buffer[i] == 'f') {
            nf++;
        }
    }

    vec3 *vertex_buffer = calloc(nv, sizeof(vec3));

    triangle *face_buffer = calloc(nf, sizeof(triangle));

    char line_buf[LOADER_MAX_LINE_LENGTH] = {0};
    char tok_buf[LOADER_MAX_LINE_LENGTH] = {0};
    int line_cursor = 0;
    int tok_cursor = 0;

    int vert_number = 0;
    int face_number = 0;

    while (tok(buffer, line_buf, '\n', &line_cursor, LOADER_MAX_LINE_LENGTH)) {
        int line_tok_num = 0;
        char line_type = 0;
        double v[3];
        int f[3];

        while(tok(line_buf, tok_buf, ' ', &tok_cursor, LOADER_MAX_LINE_LENGTH)) {
            if (line_tok_num == 0) {
                line_type = tok_buf[0];
            } else if (line_tok_num > 0) {
                if (line_type == 'v') {
                    v[line_tok_num-1] = atof(tok_buf);
                } else {
                    f[line_tok_num-1] = atoi(tok_buf);
                }
            }
            line_tok_num++;
        }

        // now we hopefully loaded either v or f
        if (line_type == 'v') {
            vertex_buffer[vert_number++] = (vec3) {v[0], v[1], v[2]};
        } else {
            face_buffer[face_number++] = (triangle) {
                vertex_buffer[f[0]],
                vertex_buffer[f[1]],
                vertex_buffer[f[2]],
            };
        }
    }

    ret.triangles = face_buffer;
    ret.num_triangles = nf;

    free(vertex_buffer);
    printf(" done\n");
    return ret;
}