#ifndef SIMULATION_H
#define SIMULATION_H

#include "Algorithm.h"
#include "Bird.h"
#include "vector"
#include <array>
#include <memory>

class Simulation
{
  private:
    std::vector<Bird> Birds;
    std::unique_ptr<Algorithm> Algo;

  public:
    void simulate();

    Simulation(std::unique_ptr<Algorithm>&& a, int birdCount);
};

#endif