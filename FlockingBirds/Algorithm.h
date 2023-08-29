#pragma once

#include "Bird.h"
#include "CSVWriter.h"
#include <vector>

class Algorithm
{
  protected:
    const int size;
    const float visionRadius;

    /// Holds all birds. The vector itself may not be modified and the sorting to be maintained, only the elements may
    /// be changed.
    std::vector<Bird> birds;

    std::vector<Obstacle> obstacles;

    std::vector<std::pair<Bird*, Vec>> leaders;

    virtual std::pair<std::vector<Bird>, std::vector<Obstacle>> objectsInVision(Bird const& bird) const;

    /// Generates a new goal for the leader.
    virtual void generateGoal(int i);

  public:
    virtual ~Algorithm() = default;
    Algorithm(const std::vector<Bird>& initialBirds, const std::vector<Obstacle>& obstacles, int leaderCount, int size, float visionRadius);

    virtual void update() = 0;
    virtual void fillCSV(CSVEntry& entry) = 0;
};