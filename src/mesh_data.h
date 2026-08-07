#ifndef MESH_DATA
#define MESH_DATA

#include "vector_funcs.h"
#include <stdint.h>

typedef struct { vec3 position; vec3 normal; }Vertex;

typedef struct { uint16_t v0,v1,v2; } Triangle;

typedef struct {
  Vertex* vertices;
  uint32_t vertices_count;

  Triangle* triangles;
  uint32_t triangles_count;

  vec3 position; vec3 rotation; vec3 scale;

} Model3D;


#endif
