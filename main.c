#include"include/pmath.h"
/*
  Fetures:
  Powerups:
 multiple balls.(if player take multiple ball powerups it duplicated balls by 8.or some thing. goes away after some time.
*/

#define WIDTH 600
#define HEIGHT 800

typedef struct {
  vec2 position;
  vec2 velocity;
}ball;


typedef struct{
  vec2 start;
  int width;
  int height;
}rect;


int main(){

  ball b={
    {WIDTH/2.0f,HEIGHT/2.0f},
    {0.0f, 0.0f},
  };

  rect r={
    {5.0f,5.0f},
    10,10
  };

  type, object;
  
  VEC_PRINT2D(&ball.position);
  VEC_PRINT2D(&ball.velocity);
}
