#!/bin/sh

gcc -o raytracer geometry.c main.c scene.c write_image.c linmath.c mesh_primitives.c load_obj.c -lm -lpng -Wall -Wno-missing-braces -Wno-unused-variable -pedantic -Wno-unused-but-set-variable