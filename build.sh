gcc core/src/main.c lib/src/write_image.c lib/src/vec.c core/src/object.c scenes/src/scenes.c core/src/camera.c -std=c11 -lopenblas -lpng -lm -Icore/inc -Ilib/inc -Iscenes/inc -o raytracer
