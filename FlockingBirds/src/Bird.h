#ifndef BIRD_H
#define BIRD_H

#include "src/Vec.h"
#include <stdio.h>

class Bird {
public:
  Vec position;
  Vec velocity;
  float angle;

  Bird() {
    angle = 0;
    position.X = 0;
    position.Y = 0;
  }

  void print() {
    printf("x = %f, y = %f and theta = %f\n", position.X, position.Y, angle);
  }
};

#endif