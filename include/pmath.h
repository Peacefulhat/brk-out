#ifndef PMATH_H
#define PMATH_H
#include<stdio.h>
#include<assert.h>
#include<string.h>
#include<math.h>
#include<stdbool.h>
#define PMA_ASSERT assert
#define real_sqrt sqrtf
#define PMA_EPSILON 1e-6f
// real number type
typedef float real;

// vector type
typedef struct{
  real x,y;
}vec2;


// vector 2d 
real vec_mag2d(vec2 vec);
void vec_normalize2d(vec2 *vec);
void vec_sum2d(vec2*result, vec2 a ,vec2 b);
void vec_scaled_add2d(vec2 *result ,vec2 a, vec2 b, real scale);
void vec_sub2d(vec2*result ,vec2 a, vec2 b);
real dot_product2d(vec2 a, vec2 b);
real dot_product_angled2d(vec2 a, vec2 b , real angle);
real cross_product2d(vec2 a, vec2 b);
real cross_product_angled2d(vec2 a, vec2 b , real angle);
real scalar_projection2d(vec2 a, vec2 b);
void vector_projection2d(vec2 *result ,vec2 a, vec2 b);
void vec_reflect2d(vec2 *result, vec2 v, vec2 n);
real vec_ang_btw2d(vec2 a, vec2 b);
void vec_translate2d(vec2 *vec, vec2 offset);
void vec_scale2d(vec2 *vec, real scale);
void vec_rotate2d(vec2 *a,real angle);
void vec_shear2d(vec2 *a,vec2 shear);
void vec_perpend2d(vec2 *result, vec2 a);
void vec_lerp2d(vec2 *result ,vec2 start, vec2 end, real t);
real vec_dist2d(vec2 a, vec2 b);
void vec_midpoint2d(vec2 *result, vec2 a, vec2 b);
void vec_clamp2d(vec2* result, real min, real max);
void vec_mul2d(vec2* result,vec2 a, vec2 b);
void vec_div2d(vec2* result, vec2 a, vec2 b);
void vec_unit_normal2d(vec2 *result, vec2 vec);
bool vec_is_zero2d(vec2 vec);
bool vec_equal2d(vec2 a, vec2 b);
real vec_signed_angle2d(vec2 a, vec2 b);
real vec_angle2d(vec2 vec);
void vec_reflect_dampe2d(vec2 *result ,vec2 v, vec2 n, real damping);

// initilize a vector
#define VEC_INIT2D(vec, x, y) do { \
    PMA_ASSERT((vec) != NULL);     \
    (vec)->x = (x);                \
    (vec)->y = (y);                \
} while(0)

// invert a vector
#define VEC_INVERT2D(vec) do {\
    PMA_ASSERT(vec!=NULL);    \
    (vec)->x=-(vec)->x;       \
    (vec)->y=-(vec)->y;       \
  }while(0)

// scale multiplicaition
#define VEC_SCALAR_MUL2D(vec,value) do{\
    PMA_ASSERT(vec!=NULL);             \
    (vec)->x*=value;                   \
    (vec)->y*=value;                   \
  }while(0)

#define VEC_PRINT2D(vec) printf("{x: %f, y: %f}\n", (vec)->x, (vec)->y)

#endif // PMATH_H


#ifdef PMATH_IMP

// calculate the magnitude
real vec_mag2d(vec2 vec){
  return real_sqrt(vec.x*vec.x+vec.y*vec.y);
}

// normalize the vector (vector into unit vector)
void vec_normalize2d(vec2 *vec){
  
  PMA_ASSERT(vec!=NULL);
  real mag=vec_mag2d(*vec);
  PMA_ASSERT(mag<PMA_EPSILON);
  
  vec->x*=(real)1/mag;
  vec->y*=(real)1/mag;
}

// sum of two vectors
void vec_sum2d(vec2*result, vec2 a ,vec2 b){
  
  PMA_ASSERT(result!=NULL);
  result->x=a.x+b.x;
  result->y=a.y+b.y;
}

// difference of two vectors
void vec_sub2d(vec2*result, vec2 a ,vec2 b){
  
  PMA_ASSERT(result!=NULL);
  result->x=a.x-b.x;
  result->y=a.y-b.y;
}

// scaled vector addition
void vec_scaled_add2d(vec2 *result,vec2 a, vec2 b, real scale){
  
  PMA_ASSERT(result!=NULL);
  VEC_SCALAR_MUL2D(&b,scale);
  result->x=a.x+b.x;
  result->y=a.y+b.y;
}

// dot product of two vectors
real dot_product2d(vec2 a, vec2 b){
  return(a.x*b.x)+(a.y*b.y);
}

// dot product of two vectors with angle between them
real dot_product_angled2d(vec2 a, vec2 b , real angle){
  return vec_mag2d(a)*vec_mag2d(b)*cosf(angle);
}

// cross product of two vectors
real cross_product2d(vec2 a, vec2 b){
  return (a.x*b.y)-(a.y*b.x);
}

// cross product of two vectors with angle between them
real cross_product_angled2d(vec2 a, vec2 b , real angle){
  return vec_mag2d(a)*vec_mag2d(b)*sinf(angle);
}

// scaler projection of a on b
real scalar_projection2d(vec2 a, vec2 b){
  return dot_product2d(a,b)/vec_mag2d(b);  
}

// vector projection of a on b
void vector_projection2d(vec2 *result ,vec2 a, vec2 b){
  PMA_ASSERT(result!=NULL);
  real b_dot_b=dot_product2d(b,b);
  PMA_ASSERT(b_dot_b!=0);
  
  real scaling=dot_product2d(a,b)/b_dot_b;
  VEC_SCALAR_MUL2D(&b, scaling);
  *result=b;
}

// vector reflected across a normal 
void vec_reflect2d(vec2 *result, vec2 v, vec2 n){
  PMA_ASSERT(result!=NULL);
  real v_dot_n=dot_product2d(v,n);
  VEC_SCALAR_MUL2D(&n,2*v_dot_n);
  vec_sub2d(result,v,n);
}

// angle between two vectors;
real vec_ang_btw2d(vec2 a, vec2 b){
  real a_dot_b=dot_product2d(a,b);
  real mag_a = vec_mag2d(a);
  real mag_b = vec_mag2d(b);
  if (mag_a == 0 || mag_b == 0) return 0;
  return acosf(a_dot_b/(mag_a*mag_b));
}


// translate vector by some offset
void vec_translate2d(vec2 *vec, vec2 offset) {
  PMA_ASSERT(vec != NULL);
  vec->x += offset.x;
  vec->y += offset.y;
}

// scale a vector by scaling factor;
void vec_scale2d(vec2 *vec, real scale) {
  PMA_ASSERT(vec != NULL);
  VEC_SCALAR_MUL2D(vec, scale);
}


// rotate vector by an angle
void vec_rotate2d(vec2 *a,real angle){
  PMA_ASSERT(a!=NULL);
  vec2 temp=*a;
  a->x=(temp.x*cosf(angle)-temp.y*sinf(angle));
  a->y=(temp.x*sinf(angle)+temp.y*cosf(angle));
}

 //shear transformation
void vec_shear2d(vec2 *a,vec2 shear){
  PMA_ASSERT(a!=NULL);
  vec2 temp=*a;
  a->x=temp.x+shear.x*temp.y;
  a->y=temp.y+shear.y*temp.x;
}

// vector perpendicluar to a
void vec_perpend2d(vec2 *result, vec2 a){
  PMA_ASSERT(result!=NULL);
  result->x=-a.y;
  result->y=a.x;
}

// learp function for finding point between 
void vec_lerp2d(vec2 *result ,vec2 start, vec2 end, real t){
  PMA_ASSERT(result!=NULL);
  result->x = (start.x)+t*(end.x-start.x);
  result->y = (start.y)+t*(end.y-start.y);
}

//distance between two vectors
real vec_dist2d(vec2 a, vec2 b){
  return real_sqrt(((b.x-a.x)*(b.x-a.x))+((b.y-a.y)*(b.y-a.y)));
}

// midpoint between a and b
void vec_midpoint2d(vec2 *result, vec2 a, vec2 b){
  PMA_ASSERT(result!=NULL);
  result->x=(a.x+b.x)/2.0f;
  result->y=(a.y+b.y)/2.0f;
}

// clamp vector value with in a range
void vec_clamp2d(vec2 *result, real min, real max) {
  PMA_ASSERT(result != NULL);
  result->x = fmaxf(min, fminf(result->x, max));
  result->y = fmaxf(min, fminf(result->y, max));
}

// component wise multiplication
void vec_mul2d(vec2* result,vec2 a, vec2 b){
  PMA_ASSERT(result != NULL);
  result->x = a.x * b.x;
  result->y = a.y * b.y;
}

// component wise division
void vec_div2d(vec2* result, vec2 a, vec2 b)
{
  PMA_ASSERT(result!=NULL);
  PMA_ASSERT(b.x!=0);
  PMA_ASSERT(b.y!=0);
  result->x=a.x/b.x;
  result->y=a.y/b.y;
}

// angle of single vector
real vec_angle2d(vec2 vec) {
  return atan2f(vec.y, vec.x);
}

// damp the reflected vector
void vec_reflect_dampe2d(vec2 *result ,vec2 v, vec2 n, real damping){
  PMA_ASSERT(result != NULL);
  real v_dot_n = dot_product2d(v, n);
  VEC_SCALAR_MUL2D(&n, 2 * v_dot_n);
  vec_sub2d(result, v, n);
  VEC_SCALAR_MUL2D(result, damping);
}

// gives a unit vector perpendicluar to vector
void vec_unit_normal2d(vec2 *result, vec2 vec){
  PMA_ASSERT(result!=NULL);
  real mag=vec_mag2d(vec);
  PMA_ASSERT(mag<PMA_EPSILON);
  result->x=vec.x/mag;
  result->y=vec.y/mag;
}

// Compute the signed angle between two vectors
real vec_signed_angle2d(vec2 a, vec2 b) {
  return atan2f(cross_product2d(a, b), dot_product2d(a, b));
}

// using epsilon based comparion for smaller value to aviod floating point precision erros

// is zero vector?
bool vec_is_zero2d(vec2 vec) {
  return (fabsf(vec.x) < PMA_EPSILON) && (fabsf(vec.y) < PMA_EPSILON);
}


bool vec_equal2d(vec2 a, vec2 b) {
  return (fabsf(a.x - b.x) < PMA_EPSILON) && (fabsf(a.y - b.y) < PMA_EPSILON);
}

#endif//PMATH_IMP
