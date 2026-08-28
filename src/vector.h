#ifndef VECTOR
#define VECTOR
#include <math.h>

// vector 2

typedef struct {
  float x,y;
} vec2;

static inline vec2 create_vec2(float _x, float _y){
  return (vec2){_x,_y};
}

static inline float get_len_vec2(vec2 v){
  return sqrtf(v.x*v.x + v.y*v.y);
}

static inline vec2 add_vec2(vec2 v1, vec2 v2){
  return (vec2){v1.x + v2.x, v1.y+v2.y};
}

static inline vec2 sub_vec2(vec2 v1, vec2 v2){
  return (vec2){v1.x - v2.x, v1.y-v2.y};
}

static inline vec2 mul_vec2(vec2 v1, vec2 v2){
  return (vec2){v1.x * v2.x, v1.y*v2.y};
}

static inline vec2 div_vec2(vec2 v1, vec2 v2){
  return (vec2){v1.x / v2.x, v1.y/v2.y};
}

static inline vec2 norm_vec2(vec2 v){
  float len = get_len_vec2(v);
  return (vec2){v.x/len, v.y/len};
}

static inline float dot_vec2(vec2 v1, vec2 v2){
  return v1.x*v2.x + v1.y*v2.y;
}

static inline vec2 ndc(vec2 vec, int width, int height){
  float aspect = (float)width/(float)height;
  float pixel_ratio = 11.0f / 24.0f;

  float x = (vec.x/(aspect*pixel_ratio) + 1.0f)/2.0f * (float)width;
  float y = (1.0f-vec.y)/2.0f * (float)height;
  return (vec2){x,y};
}

// vector 3

typedef struct {
  float x,y,z;
} vec3;

static inline vec3 create_vec3(float _x, float _y, float _z){
  return (vec3){_x,_y,_z};
}

static inline float get_len_vec3(vec3 v){
  return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

static inline vec3 add_vec3(vec3 v1, vec3 v2){
  return (vec3){v1.x + v2.x, v1.y+v2.y, v1.z+v2.z};
}

static inline vec3 sub_vec3(vec3 v1, vec3 v2){
  return (vec3){v1.x - v2.x, v1.y-v2.y, v1.z-v2.z};
}

static inline vec3 mul_vec3(vec3 v1, vec3 v2){
  return (vec3){v1.x * v2.x, v1.y*v2.y, v1.z*v2.z};
}

static inline vec3 div_vec3(vec3 v1, vec3 v2){
  return (vec3){v1.x / v2.x, v1.y/v2.y, v1.z/v2.z};
}

static inline vec3 norm_vec3(vec3 v){
  float len = get_len_vec3(v);
  return (vec3){v.x / len, v.y / len, v.z / len};
}

static inline float dot_vec3(vec3 v1, vec3 v2){
  return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

static inline vec3 cross_vec3(vec3 v1, vec3 v2){
  return (vec3){v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x};
}

static inline vec2 project(vec3 v3){
  return (vec2){v3.x/v3.z, v3.y/v3.z};
}

#endif
