#ifndef VECTOR_FUNCS
#define VECTOR_FUNCS
#include <math.h>

// vector 2

typedef struct vec2 {
  float x,y;
} vec2;

static inline vec2 create_vec2(float _x, float _y){
  vec2 v = {.x=_x,.y=_y};
  return v;
}

static inline float get_len_vec2(vec2 v){
  return sqrtf(v.x*v.x + v.y*v.y);
}

static inline vec2 add_vec2(vec2 v1, vec2 v2){
  vec2 v = {v1.x + v2.x, v1.y+v2.y};
  return v;
}

static inline vec2 sub_vec2(vec2 v1, vec2 v2){
  vec2 v = {v1.x - v2.x, v1.y-v2.y};
  return v;
}

static inline vec2 mul_vec2(vec2 v1, vec2 v2){
  vec2 v = {v1.x * v2.x, v1.y*v2.y};
  return v;
}

static inline vec2 div_vec2(vec2 v1, vec2 v2){
  vec2 v = {v1.x / v2.x, v1.y/v2.y};
  return v;
}

static inline vec2 norm_vec2(vec2 v){
  float len = get_len_vec2(v);
  vec2 res = {v.x / len, v.y / len};
  return res;
}

static inline float dot_vec2(vec2 v1, vec2 v2){
  return v1.x*v2.x + v1.y*v2.y;
}

static inline vec2 ndc(vec2 vec, int width, int height){
  float aspect = (float)width/(float)height;
  float pixel_ratio = 11.0f / 24.0f;

  float x = (vec.x/(aspect*pixel_ratio) + 1.0f)/2.0f * (float)width;
  float y = (1.0f-vec.y)/2.0f * (float)height;
  vec2 v = {x,y};
  return v;
}

// vector 3

typedef struct vec3 {
  float x,y,z;
} vec3;

static inline vec3 create_vec3(float _x, float _y, float _z){
  vec3 v = {.x=_x,.y=_y,.z=_z};
  return v;
}

static inline float get_len_vec3(vec3 v){
  return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

static inline vec3 add_vec3(vec3 v1, vec3 v2){
  vec3 v = {v1.x + v2.x, v1.y+v2.y, v1.z+v2.z};
  return v;
}

static inline vec3 sub_vec3(vec3 v1, vec3 v2){
  vec3 v = {v1.x - v2.x, v1.y-v2.y, v1.z-v2.z};
  return v;
}

static inline vec3 mul_vec3(vec3 v1, vec3 v2){
  vec3 v = {v1.x * v2.x, v1.y*v2.y, v1.z*v2.z};
  return v;
}

static inline vec3 div_vec3(vec3 v1, vec3 v2){
  vec3 v = {v1.x / v2.x, v1.y/v2.y, v1.z/v2.z};
  return v;
}

static inline vec3 norm_vec3(vec3 v){
  float len = get_len_vec3(v);
  vec3 res = {v.x / len, v.y / len, v.z / len};
  return res;
}

static inline float dot_vec3(vec3 v1, vec3 v2){
  return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

static inline vec3 cross_vec3(vec3 v1, vec3 v2){
  vec3 res;
  res.x = v1.y*v2.z - v1.z*v2.y;
  res.y = v1.z*v2.x - v1.x*v2.z;
  res.z = v1.x*v2.y - v1.y*v2.x;
  return res;
}

static inline vec2 project(vec3 v3){
  vec2 v2;
  v2.x = v3.x / v3.z; v2.y = v3.y / v3.z;
  return v2;
}

#endif
