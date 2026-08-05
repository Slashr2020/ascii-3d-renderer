#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "mesh_data.h"
#include "obj_parser.h"
#include "vector_funcs.h"
#include "matrix.h"

float clamp(float value, float max, float min){
  return fmaxf(fminf(value,max),min);
}

vec3 baricentric_coords(vec3 v0, vec3 v1, vec3 v2, float x, float y){
  float det = (v0.x - v2.x)*(v1.y - v2.y) - (v1.x - v2.x)*(v0.y - v2.y);

  if (fabsf(det)<0.001f) return create_vec3(-1.0f,-1.0f,-1.0f);

  float l0 = ((v1.y - v2.y)*(x-v2.x) + (v2.x-v1.x)*(y-v2.y))/det; 
  float l1 = ((v2.y - v0.y)*(x-v2.x) + (v0.x-v2.x)*(y-v2.y))/det; 
  float l2 = 1.0f - l0 - l1; 
  vec3 lambda = create_vec3(l0, l1, l2);
  return lambda;
}

mat4 get_model_mat(Model3D model){
  mat4 rot_x = mx_mat4(model.rotation.x);
  mat4 rot_y = my_mat4(model.rotation.y);
  mat4 rot_z = mz_mat4(model.rotation.z);
  mat4 rotation = mul_mat4(mul_mat4(rot_y, rot_x), rot_z);

  mat4 position = translation_mat(model.position.x, model.position.y, model.position.z);
  mat4 model_matrix = mul_mat4(rotation, position);
  return model_matrix;
}

void draw(int x, int y, float z, int width, int height, char* screen, float* z_buffer ,char sym){
  if (x>=0 && x<width && y>=0 && y<height){
    int z_idx = y * width + x;

    if (z < z_buffer[z_idx]){
      z_buffer[z_idx] = z;
      int scr_idx = y*(width+1)+x;
      if (screen[scr_idx]!='\n') screen[scr_idx]=sym;

    }
  }
}

void bounding_box(vec3 v0, vec3 v1, vec3 v2, vec3 normal, int width, int height, char* screen, float* z_buffer){
  float min_x = fminf(v0.x, fminf(v1.x, v2.x));
  float max_x = fmaxf(v0.x, fmaxf(v1.x, v2.x));

  float min_y = fminf(v0.y, fminf(v1.y, v2.y));
  float max_y = fmaxf(v0.y, fmaxf(v1.y, v2.y));

  if (max_x >= width) max_x = width-1; if (min_x<0) min_x=0;
  if (max_y >= height) max_y = height-1; if (min_y<0) min_y=0;

  const char* gradient = ".:-+=a%#@"; 
  int gradient_size = 10;

  float z_near = 0.0f; float z_far = 100.0f;
  vec3 light_dir = {1.0f,1.0f,0.5f};

#if 0 
  vec3 edge1 = sub_vec3(v1,v0); struct vec3 edge2 = sub_vec3(v2,v0);
  vec3 normal = norm_vec3(cross_vec3(edge2, edge1));
#endif

  float light_intensity = fmaxf(dot_vec3(normal, light_dir), 0.0f);
#if 0
  float norm_z = (z-z_near) / (z_far-z_near);
  norm_z = clamp(norm_z, 1.0f, 0.0f);
  float intensity = (1-norm_z)*light_intensity;
#endif
  int idx = (int)(light_intensity*(float)(gradient_size-1));
  if (idx>=gradient_size) idx = gradient_size-1;
  char sym = gradient[idx];


  for (int y=min_y; y<=max_y; y++){
    for (int x=min_x; x<=max_x; x++){
      vec3 lambda = baricentric_coords(v0,v1,v2,(float)x,(float)y);
      if (lambda.x>=0.0f && lambda.y>=0.0f && lambda.z>=0.0f){
        float z = (1.0f/v0.z)*lambda.x + (1.0f/v1.z)*lambda.y + (1.0f/v2.z)*lambda.z;
        float nz = 1.0f/z;
        draw(x,y,nz,width,height,screen,z_buffer,sym);
      }
    }
  }

}

void draw_model(Model3D model, int width, int height, char* screen, float* z_buffer){
  mat4 transform_mat = get_model_mat(model);
  vec3* pixel_vertices = (vec3*)malloc(model.vertices_count * sizeof(vec3));
  for (int i=0; i<model.vertices_count; i++){
    vec3 vertex = mul_vec3_mat4(model.vertices[i].position, transform_mat);
    vec2 uv = project(vertex);
    vec2 scr = ndc(uv,width,height);
    pixel_vertices[i]=create_vec3(scr.x, scr.y, vertex.z);
  }

  for (int i=0; i<model.triangles_count; i++){
    Triangle t = model.triangles[i];

    vec3 v0 = pixel_vertices[t.v0];
    vec3 v1 = pixel_vertices[t.v1];
    vec3 v2 = pixel_vertices[t.v2];

    vec3 vn0 = model.vertices[t.v0].normal;
    vec3 vn1 = model.vertices[t.v1].normal;
    vec3 vn2 = model.vertices[t.v2].normal;

    vec3 face_normal = div_vec3(add_vec3(add_vec3(vn0, vn1), vn2), (vec3){3.0f, 3.0f, 3.0f}); 

    vec3 rotated_normal = norm_vec3(mul_vec3_mat4(face_normal, transform_mat));
    bounding_box(v0,v1,v2, rotated_normal,width,height,screen,z_buffer);
  }
  free(pixel_vertices);
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
int buffer_size = (WIDTH+1)*HEIGHT+1;

int main(){
  char* screen = (char*)malloc(buffer_size*sizeof(char));
  float* z_buffer = (float*)malloc(WIDTH*HEIGHT*sizeof(float)); 
  Model3D cube = load_obj("../obj/uv_sphere.obj");
  cube.position = (vec3){0.0f, 0.0f, 2.5f};
  cube.rotation = (vec3){0,0,0};
  float angle = 0.0f;
#if 0 
  printf("vertices count: %d\n", cube.vertices_count);
  printf("triangles_count: %d\n", cube.triangles_count);
  printf("%f %f %f\n", cube.vertices[0].position.x, cube.vertices[0].position.y, cube.vertices[0].position.z);
#endif
  printf("\x1b[?25l");
  while (1) {
    clear_window(screen, z_buffer, buffer_size, WIDTH, HEIGHT);
    
    angle+=0.03f;
    cube.rotation = (vec3){angle*0.5f, angle, angle*0.25f};
    
    draw_model(cube, WIDTH, HEIGHT, screen, z_buffer);
    printf("\x1b[H"); 
    fwrite(screen, sizeof(char), buffer_size - 1, stdout);
    fflush(stdout);
    usleep(16666);
  } 
  getchar();
  return 0;
}
