#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Bird.h"
#include "Obstacle.h"
#include <vector>

class Algorithm {
public:
  virtual void update(std::vector<Bird> &birds, std::vector<Obstacle> &obs) = 0;
};

#endif