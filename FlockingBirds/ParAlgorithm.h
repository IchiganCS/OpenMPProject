#ifndef FLOCKINGBIR_H
#define FLOCKINGBIR_H
#include "Algorithm.h"
#include "Bird.h"
#include <memory>
#include <tuple>
#include <vector>

class ParAlgorithm : public Algorithm
{
  private:
    const int width;
    const int height;
    const float visionRadius;
    const float maxSpeed;


    /// Sorts the birds by their x-coordinates and overwrites the partitions.    
    void repartition();
    void reassignPartition(Bird*);
    std::vector<std::tuple<std::vector<Bird*>, float, float>> partitions;

    std::vector<Bird> birds;

    Bird* leader;
    Vec leaderGoal;
    void generateGoal();

    std::vector<Bird*> neighboursOf(const Bird* bird) const;

    

  public:
    ParAlgorithm() = delete;
    ParAlgorithm(const std::vector<Bird>& initialBirds, int width, int height, float visionRadius, float maxSpeed,
                 int partitionCount);

      

    const std::vector<Bird>& readBirds = birds;
    const Vec& readGoal = leaderGoal;
    Bird* const& readLeader = leader;

    static constexpr float leaderAttractionScale = 0.01;
    static constexpr float separationBreakOne = 15.0;
    static constexpr float cohesionScale = 0.02;
    static constexpr float goalAttraction = 10;

    virtual ~ParAlgorithm() = default;
    virtual const std::vector<Bird>& update(float delta);
};
#endif