#define TINYOBJ_LOADER_C_IMPLEMENTATION
#include "tinyobj_loader_c.h"

//#include "glad.h"

#include <float.h>
#include <limits.h>
#include <math.h>

#ifdef _WIN64
#define atoll(S) _atoi64(S)
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <unistd.h>
#endif

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <GLFW/glfw3.h>

//#include "trackball.h"

typedef struct {
  GLuint vb;
  int numTriangles;
} DrawObject;

static DrawObject gDrawObject;

static int width = 768;
static int height = 768;

static float prevMouseX, prevMouseY;
static int mouseLeftPressed;
static int mouseMiddlePressed;
static int mouseRightPressed;
static float curr_quat[4];
static float prev_quat[4];
static float eye[3], lookat[3], up[3];

static GLFWwindow* gWindow;

static int LoadObjAndConvert(float bmin[3], float bmax[3],
                             const char* filename) {
  tinyobj_attrib_t attrib;
  tinyobj_shape_t* shapes = NULL;
  size_t num_shapes;
  tinyobj_material_t* materials = NULL;
  size_t num_materials;

  {
    unsigned int flags = TINYOBJ_FLAG_TRIANGULATE;
    int ret = tinyobj_parse_obj(&attrib, &shapes, &num_shapes, &materials,
                                &num_materials, filename, get_file_data, flags);
    if (ret != TINYOBJ_SUCCESS) {
      return 0;
    }

    printf("# of shapes    = %d\n", (int)num_shapes);
    printf("# of materials = %d\n", (int)num_materials);

    /*
    {
      int i;
      for (i = 0; i < num_shapes; i++) {
        printf("shape[%d] name = %s\n", i, shapes[i].name);
      }
    }
    */
  }



  {
    DrawObject o;
    float* vb;
    /* std::vector<float> vb; //  */
    size_t face_offset = 0;
    size_t i;

    /* Assume triangulated face. */
    size_t num_triangles = attrib.num_face_num_verts;
    size_t stride = 9; /* 9 = pos(3float), normal(3float), color(3float) */

    vb = (float*)malloc(sizeof(float) * stride * num_triangles * 3);

    for (i = 0; i < attrib.num_face_num_verts; i++) {
      size_t f;
      assert(attrib.face_num_verts[i] % 3 ==
             0); /* assume all triangle faces. */
      for (f = 0; f < (size_t)attrib.face_num_verts[i] / 3; f++) {
        size_t k;
        float v[3][3];
        float n[3][3];
        float c[3];
        float len2;

        tinyobj_vertex_index_t idx0 = attrib.faces[face_offset + 3 * f + 0];
        tinyobj_vertex_index_t idx1 = attrib.faces[face_offset + 3 * f + 1];
        tinyobj_vertex_index_t idx2 = attrib.faces[face_offset + 3 * f + 2];

        for (k = 0; k < 3; k++) {
          int f0 = idx0.v_idx;
          int f1 = idx1.v_idx;
          int f2 = idx2.v_idx;
          assert(f0 >= 0);
          assert(f1 >= 0);
          assert(f2 >= 0);

          v[0][k] = attrib.vertices[3 * (size_t)f0 + k];
          v[1][k] = attrib.vertices[3 * (size_t)f1 + k];
          v[2][k] = attrib.vertices[3 * (size_t)f2 + k];
          bmin[k] = (v[0][k] < bmin[k]) ? v[0][k] : bmin[k];
          bmin[k] = (v[1][k] < bmin[k]) ? v[1][k] : bmin[k];
          bmin[k] = (v[2][k] < bmin[k]) ? v[2][k] : bmin[k];
          bmax[k] = (v[0][k] > bmax[k]) ? v[0][k] : bmax[k];
          bmax[k] = (v[1][k] > bmax[k]) ? v[1][k] : bmax[k];
          bmax[k] = (v[2][k] > bmax[k]) ? v[2][k] : bmax[k];
        }

        if (attrib.num_normals > 0) {
          int f0 = idx0.vn_idx;
          int f1 = idx1.vn_idx;
          int f2 = idx2.vn_idx;
          if (f0 >= 0 && f1 >= 0 && f2 >= 0) {
            assert(f0 < (int)attrib.num_normals);
            assert(f1 < (int)attrib.num_normals);
            assert(f2 < (int)attrib.num_normals);
            for (k = 0; k < 3; k++) {
              n[0][k] = attrib.normals[3 * (size_t)f0 + k];
              n[1][k] = attrib.normals[3 * (size_t)f1 + k];
              n[2][k] = attrib.normals[3 * (size_t)f2 + k];
            }
          } else { /* normal index is not defined for this face */
            /* compute geometric normal */
            CalcNormal(n[0], v[0], v[1], v[2]);
            n[1][0] = n[0][0];
            n[1][1] = n[0][1];
            n[1][2] = n[0][2];
            n[2][0] = n[0][0];
            n[2][1] = n[0][1];
            n[2][2] = n[0][2];
          }
        } else {
          /* compute geometric normal */
          CalcNormal(n[0], v[0], v[1], v[2]);
          n[1][0] = n[0][0];
          n[1][1] = n[0][1];
          n[1][2] = n[0][2];
          n[2][0] = n[0][0];
          n[2][1] = n[0][1];
          n[2][2] = n[0][2];
        }

        for (k = 0; k < 3; k++) {
          vb[(3 * i + k) * stride + 0] = v[k][0];
          vb[(3 * i + k) * stride + 1] = v[k][1];
          vb[(3 * i + k) * stride + 2] = v[k][2];
          vb[(3 * i + k) * stride + 3] = n[k][0];
          vb[(3 * i + k) * stride + 4] = n[k][1];
          vb[(3 * i + k) * stride + 5] = n[k][2];

          /* Use normal as color. */
          c[0] = n[k][0];
          c[1] = n[k][1];
          c[2] = n[k][2];
          len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
          if (len2 > 0.0f) {
            float len = (float)sqrt((double)len2);

            c[0] /= len;
            c[1] /= len;
            c[2] /= len;
          }

          vb[(3 * i + k) * stride + 6] = (c[0] * 0.5f + 0.5f);
          vb[(3 * i + k) * stride + 7] = (c[1] * 0.5f + 0.5f);
          vb[(3 * i + k) * stride + 8] = (c[2] * 0.5f + 0.5f);
        }
      }
      /* You can access per-face material through attrib.material_ids[i] */

      face_offset += (size_t)attrib.face_num_verts[i];
    }

    o.vb = 0;
    o.numTriangles = 0;
    if (num_triangles > 0) {
      glGenBuffers(1, &o.vb);
      glBindBuffer(GL_ARRAY_BUFFER, o.vb);
      glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(num_triangles * 3 * stride * sizeof(float)),
                   vb, GL_STATIC_DRAW);
      o.numTriangles = (int)num_triangles;
    }

    free(vb);

    gDrawObject = o;
  }