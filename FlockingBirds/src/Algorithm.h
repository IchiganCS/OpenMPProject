#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "src/Bird.h"
#include <vector>

class Algorithm {
public:
  virtual void update(std::vector<Bird> &birds) = 0;
};

#endif