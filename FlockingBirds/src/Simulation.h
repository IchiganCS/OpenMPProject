#ifndef SIMULATION_H
#define SIMULATION_H

#include "Algorithm.h"
#include "Bird.h"
#include "Obstacle.h"
#include "ParallelForAlgo.h"
#include "ParallelForAlgoWithRed.h"
#include "SimpleFlockingBird.h"
#include <vector>
class Simulation {
private:
  std::vector<Bird> &Birds1;
  std::vector<Bird> &Birds2;
  std::vector<Bird> &Birds3;
  std::vector<Obstacle> obstacles;
  Algorithm *algo1;
  Algorithm *algo2;
  Algorithm *algo3;

public:
  void simulate();
  Simulation(Algorithm *a1, Algorithm *a2, Algorithm *a3,
             std::vector<Bird> &birds, std::vector<Obstacle> &o);

  // Simulation(Algorithm *a, std::vector<Bird> &birds,
  //            std::vector<Obstacle> &obstacles);
};

#endif