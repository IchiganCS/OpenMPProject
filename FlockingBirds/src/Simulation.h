#ifndef SIMULATION_H
#define SIMULATION_H

#include "src/Algorithm.h"
#include "src/Bird.h"
#include "src/Obstacle.h"
#include <vector>

class Simulation {
private:
  std::vector<Bird> &Birds;
  std::vector<Obstacle> obstacles;
  Algorithm *algo;

public:
  void simulate();

  Simulation(Algorithm *a, std::vector<Bird> &birds,
             std::vector<Obstacle> &obstacles);
};

#endif