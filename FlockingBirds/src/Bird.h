#ifndef BIRD_H
#define BIRD_H

#include "Vec.h"
#include <cmath>
#include <stdio.h>

class Bird {
public:
  Vec position;
  Vec velocity;
  float angle;

  Bird() {
    angle = 0;
    position.x = 0;
    position.y = 0;
  }

  bool operator==(const Bird &other) const {
    return position == other.position && velocity == other.velocity &&
           angle == other.angle;
  }

  void print() {
    printf("position.x = %f, position.y = %f, velocity.x = %f, velocity.y = "
           "%f, theta = %f\n",
           position.x, position.y, velocity.x, velocity.y, angle);
  }
  double calculateDirection() { return std::atan2(velocity.y, velocity.x); }
};

#endif