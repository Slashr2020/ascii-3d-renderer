#ifndef VECTOR_FUNCS
#define VECTOR_FUNCS
#include <math.h>

// vector 2

typedef struct vec2 {
  float x,y;
} vec2;

static inline struct vec2 create_vec2(float _x, float _y){
  struct vec2 v = {.x=_x,.y=_y};
  return v;
}

static inline float get_len_vec2(struct vec2 v){
  return sqrtf(v.x*v.x + v.y*v.y);
}

static inline struct vec2 add_vec2(struct vec2 v1, struct vec2 v2){
  struct vec2 v = {v1.x + v2.x, v1.y+v2.y};
  return v;
}

static inline struct vec2 sub_vec2(struct vec2 v1, struct vec2 v2){
  struct vec2 v = {v1.x - v2.x, v1.y-v2.y};
  return v;
}

static inline struct vec2 mul_vec2(struct vec2 v1, struct vec2 v2){
  struct vec2 v = {v1.x * v2.x, v1.y*v2.y};
  return v;
}

static inline struct vec2 div_vec2(struct vec2 v1, struct vec2 v2){
  struct vec2 v = {v1.x / v2.x, v1.y/v2.y};
  return v;
}

static inline struct vec2 norm_vec2(struct vec2 v){
  float len = get_len_vec2(v);
  struct vec2 res = {v.x / len, v.y / len};
  return res;
}

static inline float dot_vec2(struct vec2 v1, struct vec2 v2){
  return v1.x*v2.x + v1.y*v2.y;
}

static inline struct vec2 ndc(struct vec2 vec, int width, int height){
  float aspect = (float)width/(float)height;
  float pixel_ratio = 11.0f / 24.0f;

  float x = (vec.x/(aspect*pixel_ratio) + 1.0f)/2.0f * (float)width;
  float y = (1.0f-vec.y)/2.0f * (float)height;
  struct vec2 v = {x,y};
  return v;
}

// vector 3

typedef struct vec3 {
  float x,y,z;
} vec3;

static inline struct vec3 create_vec3(float _x, float _y, float _z){
  struct vec3 v = {.x=_x,.y=_y,.z=_z};
  return v;
}

static inline float get_len_vec3(struct vec3 v){
  return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

static inline struct vec3 add_vec3(struct vec3 v1, struct vec3 v2){
  struct vec3 v = {v1.x + v2.x, v1.y+v2.y, v1.z+v2.z};
  return v;
}

static inline struct vec3 sub_vec3(struct vec3 v1, struct vec3 v2){
  struct vec3 v = {v1.x - v2.x, v1.y-v2.y, v1.z-v2.z};
  return v;
}

static inline struct vec3 mul_vec3(struct vec3 v1, struct vec3 v2){
  struct vec3 v = {v1.x * v2.x, v1.y*v2.y, v1.z*v2.z};
  return v;
}

static inline struct vec3 div_vec3(struct vec3 v1, struct vec3 v2){
  struct vec3 v = {v1.x / v2.x, v1.y/v2.y, v1.z/v2.z};
  return v;
}

static inline struct vec3 norm_vec3(struct vec3 v){
  float len = get_len_vec3(v);
  struct vec3 res = {v.x / len, v.y / len, v.z / len};
  return res;
}

static inline float dot_vec3(struct vec3 v1, struct vec3 v2){
  return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

static inline struct vec2 project(struct vec3 v3, float distance){
  struct vec2 v2;
  v3.z += distance;

  v2.x = v3.x / v3.z; v2.y = v3.y / v3.z;
  return v2;
}

#endif
