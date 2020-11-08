#include "mesh_primitives.h"
#include "geometry.h"
#include <stdlib.h>

object make_cube(vec3 colour, mat4 transform) {
    // 6 faces, 12 triangles
    triangle *tris = calloc(12, sizeof(triangle));

    // front face
    tris[0] = (triangle) {
        (vec3) {-1, 1, 1},
        (vec3) {1, 1, 1},
        (vec3) {-1, -1, 1}
    };    

    tris[1] = (triangle) {
        (vec3) {1, 1, 1},
        (vec3) {1, -1, 1},
        (vec3) {-1, -1, 1}
    };    
    
    // back face
    tris[2] = (triangle) {
        (vec3) {-1, 1, -1},
        (vec3) {1, 1, -1},
        (vec3) {-1, -1, -1}
    };    

    tris[3] = (triangle) {
        (vec3) {1, 1, -1},
        (vec3) {1, -1, -1},
        (vec3) {-1, -1, -1}
    };

    // left face
    tris[4] = (triangle) {
        (vec3) {-1, 1, -1},
        (vec3) {-1, 1, 1},
        (vec3) {-1, -1, -1}
    };
    
    tris[5] = (triangle) {
        (vec3) {-1, -1, 1},
        (vec3) {-1, 1, 1},
        (vec3) {-1, -1, -1}
    };
    
    // right face
    tris[6] = (triangle) {
        (vec3) {1, 1, -1},
        (vec3) {1, 1, 1},
        (vec3) {1, -1, -1}
    };    
    
    tris[7] = (triangle) {
        (vec3) {1, -1, 1},
        (vec3) {1, 1, 1},
        (vec3) {1, -1, -1}
    };
    
    // top face
    tris[8] = (triangle) {
        (vec3) {-1, 1, -1},
        (vec3) {1, 1, -1},
        (vec3) {1, 1, 1}
    };
    
    tris[9] = (triangle) {
        (vec3) {-1, 1, -1},
        (vec3) {-1, 1, 1},
        (vec3) {1, 1, 1}
    };

    // bottom face
    tris[10] = (triangle) {
        (vec3) {-1, -1, -1},
        (vec3) {1, -1, -1},
        (vec3) {1, -1, 1}
    };

    tris[11] = (triangle) {
        (vec3) {-1, -1, -1},
        (vec3) {-1, -1, 1},
        (vec3) {1, -1, 1}
    };

    printf("cube transform:\n");
    print_mat4(transform);

    for (int i = 0; i < 12 * 3; i++) {
        vec3 *as_vecs = (vec3*)tris;
        as_vecs[i] = vec3_transform(transform, as_vecs[i]);
    }
    
    return (object) {
        .colour = colour,
        .tag = OBJ_MESH,
        .mesh = {
            .num_triangles = 12,
            .triangles = tris,
        },
    };

}
