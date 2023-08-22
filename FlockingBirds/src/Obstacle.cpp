#include "Obstacle.h"
#include <cmath>

float Obstacle::distanceTo(const Vec &birdPosition) const {
  float res = std::fmax((center - birdPosition).length() - radius, 0.0f);

  return res;
}