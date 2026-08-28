#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "mesh_data.h"
#include "vector.h"
#include "obj_parser.h"

Model3D load_obj(const char* filename){
  Model3D model = {0};

  FILE *file = fopen(filename, "r");
  if (file == NULL) return model;
  
  model.vertices = malloc(4096*sizeof(Vertex));
  model.triangles = malloc(4096*sizeof(Triangle));

  model.triangles_count=0, model.vertices_count=0;

  vec3 tmp_v[1024], tmp_vn[1024];
  uint32_t count_v=0, count_vn=0;

  char line[256];
  while (fgets(line, sizeof(line), file)){
    if (strncmp(line, "v ",2)==0){
      sscanf(line,"v %f %f %f", &tmp_v[count_v].x, &tmp_v[count_v].y, &tmp_v[count_v].z);
      count_v++;
    }
    else if (strncmp(line, "vn ",3)==0){
      sscanf(line,"vn %f %f %f", &tmp_vn[count_vn].x, &tmp_vn[count_vn].y, &tmp_vn[count_vn].z);
      count_vn++;
    }
    else if (strncmp(line, "f ",2)==0){
      int v1, vn1, v2, vn2, v3, vn3;
      sscanf(line, "f %d/%*d/%d %d/%*d/%d %d/%*d/%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);
      v1--; vn1--; v2--; vn2--; v3--; vn3--;
      
      model.vertices[model.vertices_count].position = tmp_v[v1];
      model.vertices[model.vertices_count].normal = tmp_vn[vn1];
      int idx1 = model.vertices_count;
      model.vertices_count++;

      model.vertices[model.vertices_count].position = tmp_v[v2];
      model.vertices[model.vertices_count].normal = tmp_vn[vn2];
      int idx2 = model.vertices_count;
      model.vertices_count++;

      model.vertices[model.vertices_count].position = tmp_v[v3];
      model.vertices[model.vertices_count].normal = tmp_vn[vn3];
      int idx3 = model.vertices_count;
      model.vertices_count++;


      model.triangles[model.triangles_count].v0 = idx1;
      model.triangles[model.triangles_count].v1 = idx2;
      model.triangles[model.triangles_count].v2 = idx3;
      model.triangles_count++;
    }
  }


  fclose(file);
  return model;
}
