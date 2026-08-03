#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "vector_funcs.h"
#include "matrix.h"

float clamp(float value, float max, float min){
  return fmaxf(fminf(value,max),min);
}

struct vec3 baricentric_coords(vec3 v0, vec3 v1, vec3 v2, float x, float y){
  float det = (v0.x - v2.x)*(v1.y - v2.y) - (v1.x - v2.x)*(v0.y - v2.y);

  float l0 = ((v1.y - v2.y)*(x-v2.x) + (v2.x-v1.x)*(y-v2.y))/det; 
  float l1 = ((v2.y - v0.y)*(x-v2.x) + (v0.x-v2.x)*(y-v2.y))/det; 
  float l2 = 1.0f - l0 - l1; 
  struct vec3 lambda = create_vec3(l0, l1, l2);
  return lambda;
}

struct Triangle {
  int v0,v1,v2;
};

struct Model3D {
  struct vec3* vertices;
  int vertices_count;

  struct Triangle* triangles;
  int triangles_count;

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

void bounding_box(vec3 v0, vec3 v1, vec3 v2, int width, int height, char* screen, float* z_buffer){
  float min_x = fminf(v0.x, fminf(v1.x, v2.x));
  float max_x = fmaxf(v0.x, fmaxf(v1.x, v2.x));

  float min_y = fminf(v0.y, fminf(v1.y, v2.y));
  float max_y = fmaxf(v0.y, fmaxf(v1.y, v2.y));

  if (max_x >= width) max_x = width-1; if (min_x<0) min_x=0;
  if (max_y >= height) max_y = height-1; if (min_y<0) min_y=0;

  const char* gradient = ".:-+=a%#@"; 
  int gradient_size = 10;

  float z_near = -0.5f; float z_far = 0.5f;
  struct vec3 light_dir = {1.0f,1.0f,0.5f};
  
  struct vec3 edge1 = sub_vec3(v1,v0); struct vec3 edge2 = sub_vec3(v2,v0);
  struct vec3 normal = norm_vec3(cross_vec3(edge2, edge1));

  float light_intensity = fmaxf(dot_vec3(normal, light_dir), 0.0f);

  for (int y=min_y; y<=max_y; y++){
    for (int x=min_x; x<=max_x; x++){
      struct vec3 lambda = baricentric_coords(v0,v1,v2,(float)x,(float)y);
      if (lambda.x>=0.0f && lambda.y>=0.0f && lambda.z>=0.0f){
        float z = v0.z*lambda.x + v1.z*lambda.y + v2.z*lambda.z;
        float norm_z = (z-z_near) / (z_far-z_near);
        norm_z = clamp(norm_z, 1.0f, 0.0f);
        float intensity = (1-norm_z)*light_intensity;
        int idx = (int)(intensity*(float)(gradient_size-1));
        char sym = gradient[idx];

        draw(x,y,z,width,height,screen,z_buffer,sym);
      }
    }
  }

}
#if 0
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
#endif

void draw_model(struct Model3D model, int width, int height, char* screen, float* z_buffer){
  struct mat4 transform_mat = get_model_mat(model);
  struct vec3 pixel_vertices[model.vertices_count];
  for (int i=0; i<model.vertices_count; i++){
    struct vec3 vertex = mul_vec3_mat4(model.vertices[i], transform_mat);
    struct vec2 uv = project(vertex, 1.5f);
    struct vec2 scr = ndc(uv,width,height);

    pixel_vertices[i] = create_vec3(scr.x, scr.y, vertex.z);
  }
  for (int i=0; i<model.triangles_count; i++){
    struct Triangle t = model.triangles[i];
    struct vec3 v0 = pixel_vertices[t.v0];
    struct vec3 v1 = pixel_vertices[t.v1];
    struct vec3 v2 = pixel_vertices[t.v2];

    bounding_box(v0,v1,v2,width,height,screen,z_buffer);
#if 0
    line(v0, v1, screen, z_buffer, width, height);    
    line(v2, v1, screen, z_buffer, width, height);    
    line(v2, v0, screen, z_buffer, width, height);    
#endif
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

struct Triangle CUBE_TRIANGLES[12] ={
  {0,4,5}, {0,5,1}, // down
  {3,0,1}, {3,1,2}, // back
  {4,7,6}, {4,6,5}, // front
  {7,3,2}, {7,2,6}, // up
  {3,7,4}, {3,4,0}, // left
  {1,5,6}, {1,6,2}, // right
};

int buffer_size = (WIDTH+1)*HEIGHT+1;

int main(){
  char* screen = (char*)malloc(buffer_size*sizeof(char));
  float* z_buffer = (float*)malloc(WIDTH*HEIGHT*sizeof(float)); 
  struct Model3D cube1;
  cube1.vertices = CUBE_VERTICES;
  cube1.vertices_count = 8;
  cube1.triangles = CUBE_TRIANGLES;
  cube1.triangles_count = 12;
  cube1.position = (struct vec3){0.0f, 0.0f, 0.3f};
  cube1.rotation = (struct vec3){0,0,0};
  float angle = 0.0f;

  while (1) {
    clear_window(screen, z_buffer, buffer_size, WIDTH, HEIGHT);
    
    angle+=0.03f;
    cube1.rotation = (struct vec3){angle*0.5f, angle, angle*0.25f};
    
    draw_model(cube1, WIDTH, HEIGHT, screen, z_buffer);
    printf("\x1b[H%s",screen);
    usleep(30000);
  } 
  getchar();
  return 0;
}
