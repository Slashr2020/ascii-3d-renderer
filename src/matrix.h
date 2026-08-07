#ifndef MATRIX
#define MATRIX
#include "vector_funcs.h"
#include <math.h>
typedef struct mat4 {
  float m[16];
}mat4;

static inline struct mat4 mat4_identity(){
  mat4 mat = {0};
  mat.m[0] = 1.0f; mat.m[5] = 1.0f; mat.m[10] = 1.0f; mat.m[15] = 1.0f;
  return mat;
}

static inline struct mat4 mul_mat4(mat4 a, mat4 b){
  mat4 res = {0};
  for (int r=0; r<4; r++){
    for (int c=0; c<4; c++){
      res.m[r*4 + c] =
        a.m[r*4] * b.m[c] +
        a.m[r*4 + 1] * b.m[1*4 + c] +
        a.m[r*4 + 2] * b.m[2*4 + c] +
        a.m[r*4 + 3] * b.m[3*4 + c];
    }
  }
  return res;
}

static inline struct mat4 mx_mat4(float angle){
  mat4 mat = mat4_identity();
  float c = cosf(angle);
  float s = sinf(angle);

  mat.m[5] = c; mat.m[6] = -s;
  mat.m[9] = s; mat.m[10] = c;
  return mat;
}

static inline struct mat4 my_mat4(float angle){
  mat4 mat = mat4_identity();
  float c = cosf(angle);
  float s = sinf(angle);

  mat.m[0] = c; mat.m[2] = s;
  mat.m[8] = -s; mat.m[10] = c;
  return mat;
}

static inline struct mat4 mz_mat4(float angle){
  mat4 mat = mat4_identity();
  float c = cosf(angle);
  float s = sinf(angle);

  mat.m[0] = c; mat.m[1] = -s;
  mat.m[4] = s; mat.m[5] = c;
  return mat;
}

static inline struct vec3 mul_vec3_mat4(vec3 vec, mat4 mat){
  vec3 res;

  res.x = vec.x*mat.m[0] + vec.y*mat.m[4] + vec.z*mat.m[8] + mat.m[12];
  res.y = vec.x*mat.m[1] + vec.y*mat.m[5] + vec.z*mat.m[9] + mat.m[13];
  res.z = vec.x*mat.m[2] + vec.y*mat.m[6] + vec.z*mat.m[10] + mat.m[14];
  return res;
}

static inline struct mat4 translation_mat(float dx, float dy, float dz){
  mat4 mat = mat4_identity();
  mat.m[12] = dx; mat.m[13] = dy; mat.m[14] = dz;
  return mat;
}

static inline struct mat4 scale_mat(float sx, float sy, float sz){
  mat4 mat = mat4_identity();
  mat.m[0]=sx; mat.m[5]=sy; mat.m[10]=sz;
  return mat;
}

#endif
