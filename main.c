#include "scene.h"
#include "linmath.h"
#include "geometry.h"
#include "mesh_primitives.h"
#include "load_obj.h"

#include <string.h>
#include <stdio.h>

#define XRES 1280
#define YRES 960


int main(int argc, char **argv) {

    // define a test scene
    scene test_scene = {0};
    test_scene.cameras[0] = cam_mat_lookat(
        new_vec3(-1,0,3),
        new_vec3(0,1,0),
        new_vec3(0,0,-1)
    );
    // look directly right
    test_scene.cameras[1] = cam_mat_lookat(
        new_vec3(0,0,0),
        new_vec3(0,1,0),
        new_vec3(1,0,0)
    );
    test_scene.cameras[2] = cam_mat_lookat(
        new_vec3(2,0, 1),
        new_vec3(0,1,0),
        new_vec3(1,0,-1)
    );

    test_scene.objects[0] = (object) {
        .tag = OBJ_SPHERE,
        .colour = (vec3) {1,0,0},
        .sphere = (sphere) {
            .center = (vec3) {0,0,-1},
            .radius = 0.3,
        }
    };    
    test_scene.objects[1] = (object) {
        .tag = OBJ_SPHERE,
        .colour = (vec3) {0,1,0},
        .sphere = (sphere) {
            .center = (vec3) {1,0,-1},
            .radius = 0.3,
        }
    };
    test_scene.objects[2] = (object) {
        .tag = OBJ_SPHERE,
        .colour = (vec3) {0,0,1},
        .sphere = (sphere) {
            .center = (vec3) {0,1,-1},
            .radius = 0.3,
        }
    };
    test_scene.objects[3] = (object) {
        .tag = OBJ_SPHERE,
        .colour = (vec3) {1,0,1},
        .sphere = (sphere) {
            .center = (vec3) {0,0,-3},
            .radius = 1.0,
        }
    };

    test_scene.objects[4] = (object) {
        .tag = OBJ_SPHERE,
        .colour = (vec3) {0,1,1},
        .sphere = (sphere) {
            .center = (vec3) {2,0,-1},
            .radius = 0.3,
        }
    };

    test_scene.objects[5] = (object) {
        .tag = OBJ_SPHERE,
        .colour = (vec3) {1,1,1},
        .sphere = (sphere) {
            .center = (vec3) {3,0,-1},
            .radius = 0.3,
        }
    };

    test_scene.lights[0] = (point_light) {
        .pos = (vec3) {0, 5, 0},
        .colour = (vec3) {1, 0.95, 0.8}, // nice and warm
        .intensity = 1, // idk sure
    };









    scene light_test_scene = {0};
    light_test_scene.cameras[0] = cam_mat_lookat(
        (vec3){0,0,3},
        (vec3){0,1,0},
        (vec3){0,0,-1}
    );


    light_test_scene.objects[0] = (object) {
        .tag = OBJ_SPHERE,
        .colour = (vec3) {1, 0, 0.2},
        .sphere = (sphere) {
            .center = (vec3) {0, 1, -1},
            .radius = 0.8,
        }
    };

    light_test_scene.objects[1] = (object) {
        .tag = OBJ_SPHERE,
        .colour = (vec3) {0.2, 0, 1},
        .sphere = (sphere) {
            .center = (vec3) {0, -1, -1},
            .radius = 0.8,
        }
    };

/*
    light_test_scene.objects[0] = (object) {
        .tag = OBJ_TRIANGLE,
        .colour = (vec3) {1, 0, 0.2},
        .triangle = (triangle) {
            .vert = {
                {0, 1, 1},
                {0, 1, -1},
                {0, -1, -1},
            }
        }
    };
*/
    // red light on the right
    light_test_scene.lights[0] = (point_light) {
        .pos = (vec3) {3, 0, -1},
        .colour = (vec3) {1, 0, 0},
        .intensity = 20,
    };

    // blue light on the left
    light_test_scene.lights[1] = (point_light) {
        .pos = (vec3) {-3, 0, -1},
        .colour = (vec3) {0, 0, 1},
        .intensity = 20,
    };

    scene triangle_scene = {0};
    triangle_scene.cameras[0] = cam_mat_lookat(
        (vec3){0,0,5},
        (vec3){0,1,0},
        (vec3){0,0,-1}
    );

    triangle_scene.objects[0] = (object) {
        .tag = OBJ_TRIANGLE,
        .colour = (vec3) {1, 0.2, 0.5},
        .triangle = (triangle) {
            .vert = {
                {-1.0001,1,-2},
                {0.001,0,-1},
                {-1.001,0,-1},
            }
        }
    };

    triangle_scene.objects[1] = (object) {
        .tag = OBJ_TRIANGLE,
        .colour = (vec3) {0.2, 1, 0.5},
        .triangle = (triangle) {
            .vert = {
                {1,0,-1},
                {0,1,-2},
                {0,0,-1},
            }
        }
    };

    scene cube_scene = {0};
    cube_scene.cameras[0] = cam_mat_lookat(
        (vec3){-3,3,6},
        (vec3){0,1,0},
        (vec3){0,0,0}
    );    
    cube_scene.cameras[1] = cam_mat_lookat(
        (vec3){-2,-2,-4},
        (vec3){0,1,0},
        (vec3){0,0,0}
    );
    cube_scene.objects[0] = make_cube((vec3){1,0.2,1}, transformation_mat(-1,-1,-1,45,45,45,2));
    
    cube_scene.objects[1] = (object) {
        .tag = OBJ_SPHERE,
        .colour = {1,1,1},
        .sphere = {
            .center = (vec3) {5, 0, 0},
            .radius = 2,
        },
    };

    scene teapot_scene = {0};
    teapot_scene.cameras[0] = cam_mat_lookat(
        (vec3){0,5,5},
        (vec3){0,1,0},
        (vec3){0,0,0}
    );
    teapot_scene.objects[0] = (object) {
        .tag = OBJ_MESH,
        .colour = {1,1,1},
        .mesh = load_obj("teapot.obj")
    };

    scene ls2 = {0};
    ls2.cameras[0] = cam_mat_lookat(
        (vec3){0,5,10},
        (vec3){0,1,0},
        (vec3){0,0,0}
    );
    ls2.cameras[1] = cam_mat_lookat(
        (vec3){-6,3,10},
        (vec3){0,1,0},
        (vec3){0,0,0}
    );
    ls2.cameras[2] = cam_mat_lookat(
        (vec3){-6,3,-10},
        (vec3){0,1,0},
        (vec3){0,0,0}
    );    
    ls2.cameras[3] = cam_mat_lookat(
        (vec3){-6,3,-10},
        (vec3){0,1,0},
        (vec3){0,0,0}
    );

    // ground
    ls2.objects[0] = (object) {
        .tag = OBJ_TRIANGLE,
        .colour = {0.7,0.7,0.7},
        .triangle = {
            .vert = {
                (vec3){-10,0,-10},
                (vec3){10,0,10},
                (vec3){10,0,-10},
            }
        }
    };
    ls2.objects[1] = (object) {
        .tag = OBJ_TRIANGLE,
        .colour = {0.7,0.7,0.7},
        .triangle = {
            .vert = {
                (vec3){-10,0,-10},
                (vec3){10,0,10},
                (vec3){-10,0,10},
            }
        }
    };
    ls2.objects[2] = (object) {
        .tag = OBJ_SPHERE,
        .colour = {0.7,0.7,0.7},
        .sphere = {
            .center = (vec3){0,2,6},
            .radius = 1,
        }
    };
    ls2.objects[3] = make_cube((vec3){0.7,0.1,0.1}, transformation_mat(-3, 1, 2.5, 0, 45, 0, 1));
    ls2.objects[4] = make_cube((vec3){0.1,0.1,0.7}, transformation_mat(3, 1, -4, 0, -20, 0, 1));
    ls2.lights[0] = (point_light) {
        .pos = (vec3){-10, 10, 10},
        .colour = {1, 0.8, 0.7},
        .intensity = 50,
    };
    ls2.lights[1] = (point_light) {
        .pos = (vec3){10, 8, 10},
        .colour = {0.7, 0.8, 1},
        .intensity = 45,
    };

    //render_scene(&teapot_scene, "teapot.png", XRES, YRES, 0, 70, 4./3, SHADING_GOOCH);

    //render_scene(&triangle_scene, "tri.png", XRES, YRES, 0, 70, 4./3, SHADING_GOOCH);
    //render_scene(&cube_scene, "cube1.png", XRES, YRES, 0, 70, 4./3, SHADING_GOOCH);
    //render_scene(&cube_scene, "cube2.png", XRES, YRES, 1, 70, 4./3, SHADING_GOOCH);
    
    //render_scene(&test_scene, "cam1normal.png",  XRES, YRES, 0, 70, 4./3, SHADING_NORMALS);
    //render_scene(&test_scene, "cam1gooch.png",   XRES, YRES, 0, 70, 4./3, SHADING_GOOCH);
    //render_scene(&test_scene, "out2.png", XRES, YRES, 1, 80, 4./3, 0.001, 100);
    //render_scene(&test_scene, "out3.png", XRES, YRES, 2, 80, 4./3, 0.001, 100);
    
    //render_scene(&light_test_scene, "lightsa.png", XRES, YRES, 0, 80, 4./3, SHADING_AMBIENT);
    //render_scene(&light_test_scene, "lights.png", XRES, YRES, 0, 80, 4./3, SHADING_LIGHTS);
    
    render_scene(&ls2, "ls21e.png", XRES, YRES, 0, 60, 4./3, SHADING_LIGHTS);
    render_scene(&ls2, "ls22e.png", XRES, YRES, 0, 60, 4./3, SHADING_GOOCH);
    render_scene(&ls2, "ls23e'.png", XRES, YRES, 1, 60, 4./3, SHADING_LIGHTS);
    render_scene(&ls2, "ls24e.png", XRES, YRES, 2, 60, 4./3, SHADING_LIGHTS);
    render_scene(&ls2, "ls25e.png", XRES, YRES, 2, 60, 4./3, SHADING_GOOCH);
}