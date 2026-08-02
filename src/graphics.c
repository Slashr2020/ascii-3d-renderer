#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "vector_funcs.h"
#include "matrix.h"

#if 0
float clamp(float value, float max, float min){
  return fmaxf(fminf(value,max),min);
}
#endif

struct Edge {
  int start; int end;
};

struct Model3D {
  struct vec3* vertices;
  int vertices_count;

  struct Edge* edges;
  int edges_count;

  struct vec3 position;
  struct vec3 rotation;
};

struct mat4 get_model_mat(struct Model3D model){
  struct mat4 rot_x = mx_mat4(model.rotation.x);
  struct mat4 rot_y = my_mat4(model.rotation.y);
  struct mat4 rot_z = mz_mat4(model.rotation.z);
  struct mat4 rotation = mul_mat4(mul_mat4(rot_y, rot_x), rot_z);

  struct mat4 position = translation_mat(model.position.x, model.position.y, model.position.z);
  struct mat4 model_matrix = mul_mat4(rotation, position);
  return model_matrix;
}

void draw(int x, int y, float z, int width, int height, char* screen, float* z_buffer ,char sym){
  if (x>=0 && x<width && y>=0 && y<height){
    int z_idx = y * width + x;

    if (z < z_buffer[z_idx]){
      z_buffer[z_idx] = z;
      screen[y * (width+1) + x]=sym;
    }
  }

}

static inline void line(struct vec3 v0, struct vec3 v1,char* screen, float* z_buffer,int width,int height){
  int x0 = (int)v0.x; int x1 = (int)v1.x;
  int y0 = (int)v0.y; int y1 = (int)v1.y;

  int dx = abs(x1-x0); int dy = abs(y1-y0);
  int sx = x0<x1 ? 1 : -1; int sy = y0<y1 ? 1 : -1;
  int error = dx - dy;
  
  int total = dx > dy ? dx : dy;
  int step=0;

  while (1) {
    float t = (total==0) ? 1.0f : (float)step / total;
    float z0 = v0.z + (v1.z-v0.z)*t;

    draw(x0, y0, z0, width, height, screen, z_buffer, '@');
    if (x0==x1 && y0==y1) break;

    int e2 = 2*error;
    if (e2 > -dy){
      error -= dy;
      x0 += sx;
    }
    if (e2 < dx){
      error += dx;
      y0 += sy;
    }
    step++;
  }
}

void draw_model(struct Model3D model, int width, int height, char* screen, float* z_buffer){
  struct mat4 transform_mat = get_model_mat(model);
  struct vec3 pixel_vertices[model.vertices_count];
  for (int i=0; i<model.vertices_count; i++){
    struct vec3 vertex = mul_vec3_mat4(model.vertices[i], transform_mat);
    struct vec2 uv = project(vertex, 1.5f);
    struct vec2 scr = ndc(uv,width,height);

    pixel_vertices[i] = create_vec3(scr.x, scr.y, vertex.z);
  }
  for (int i=0; i<model.edges_count; i++){
    struct vec3 v0 = pixel_vertices[model.edges[i].start];
    struct vec3 v1 = pixel_vertices[model.edges[i].end];

    line(v0, v1, screen, z_buffer, width, height);    
  }
}

void clear_window(char* screen, float* z_buffer, int buffer_size, int width, int height){
  for (int i=0; i<buffer_size; i++){
    screen[i]=' ';
  }
  screen[buffer_size-1]='\0';

  for (int y=0; y<height; y++){
    screen[y * (width+1) + width]= '\n';
  }

  for (int i=0; i<width*height; i++){
    z_buffer[i] = FLT_MAX;
  }
}

#define WIDTH 120
#define HEIGHT 30

struct vec3 CUBE_VERTICES[8] = {
  { -0.5f,-0.5f,-0.5f}, {-0.5f, 0.5f, -0.5f},
  { -0.5f, 0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f},
  {0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, -0.5f},
  {0.5f, 0.5f, 0.5f}, {0.5f, -0.5f, 0.5f},

};

struct Edge CUBE_EDGES[12] = {
  {0,1}, {1,2}, {2,3}, {3,0},
  {4,5}, {5,6}, {6,7}, {7,4},
  {0,4}, {1,5}, {2,6}, {3,7}

};

int buffer_size = (WIDTH+1)*HEIGHT+1;

int main(){
  char* screen = (char*)malloc(buffer_size*sizeof(char));
  float* z_buffer = (float*)malloc(WIDTH*HEIGHT*sizeof(float)); 
  struct Model3D cube1;
  cube1.vertices = CUBE_VERTICES;
  cube1.vertices_count = 8;
  cube1.edges = CUBE_EDGES;
  cube1.edges_count = 12;
  cube1.position = (struct vec3){0.0f, 0.0f, 0.3f};
  cube1.rotation = (struct vec3){0,0,0};
  float angle = 0.0f;

  while (1) {
    clear_window(screen, z_buffer, buffer_size, WIDTH, HEIGHT);
    
    angle+=0.03f;
    cube1.rotation = (struct vec3){0.0f, angle, 0.0f};


    draw_model(cube1, WIDTH, HEIGHT, screen, z_buffer);
    printf("\x1b[H%s",screen);
    usleep(30000);
  } 
  getchar();
  return 0;
}
