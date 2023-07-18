#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Vec.h"
class Obstacle {
public:
  Vec center;
  float radius;

  float distanceTo(const Vec &position) const;
};

#endif