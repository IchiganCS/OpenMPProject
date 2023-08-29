#pragma once
#include "Algorithm.h"
#include "Bird.h"
#include "Obstacle.h"

#include <vector>

class SimpleAlgorithm : public Algorithm
{
  public:
    SimpleAlgorithm(const std::vector<Bird>& initialBirds, const std::vector<Obstacle>& obstacles, int leaderCount,
                    int size, float visionRadius);
    virtual ~SimpleAlgorithm() = default;

    virtual void update();
    virtual void fillCSV(CSVEntry& entry);
};
