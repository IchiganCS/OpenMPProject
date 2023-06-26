#ifndef SIMULATION_H
#define SIMULATION_H

#include "src/Algorithm.h"
#include "src/Bird.h"
#include <vector>

class Simulation {
private:
  std::vector<Bird> &Birds;
  Algorithm *algo;

public:
  void simulate();

  Simulation(Algorithm *a, std::vector<Bird> &birds);
};

#endif