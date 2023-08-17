#pragma once
#include "Algorithm.h"
#include "Bird.h"
#include "Obstacle.h"

#include <vector>

class SimpleAlgorithm : public Algorithm
{
  private:
    std::vector<Bird> birds;
    std::vector<Obstacle> obstacles;
  public:
    SimpleAlgorithm(std::vector<Bird> const& birds, std::vector<Obstacle> const& obs);
    virtual ~SimpleAlgorithm() = default;
    virtual void update();
    virtual void fillCSV(CSVEntry& entry);
};
