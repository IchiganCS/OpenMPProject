#ifndef FLOCKINGBIR_H
#define FLOCKINGBIR_H
#include "Algorithm.h"
#include "Bird.h"
#include "Obstacle.h"
#include <map>
#include <memory>
#include <tuple>
#include <vector>

class ParAlgorithm : public Algorithm
{
  private:
    const int width;
    const int height;
    const float visionRadius;
    const float partitionOverloadTolerance;
    const int partitionMaxElements;

    /// Sorts the birds by their x-coordinates and overwrites the partitions.
    void repartition();

    /// Checks if any partition exceeds the allowed amount of birds and if
    /// so, calls for a repartition.
    void repartitionIfOverloaded();

    /// Looks for a partition which may hold the bird using the min and max properties of the parititions.
    /// The bird is assumed to be already removed from any previous partition.
    void reassignPartition(Bird*);

    /// A vector of partitions.
    /// Each partition stores a vector of pointer to birds (pointer into the "birds" vector) as well as a minimal x and
    /// a maximal x for which it is responsible.
    std::vector<std::tuple<std::vector<Bird*>, float, float>> partitions;

    /// Holds all birds. The vector itself may not be modified, only the content.
    std::vector<Bird> birds;

    std::vector<Obstacle> obstacles;

    std::vector<std::pair<Bird*, Vec>> leaders;

    /// Generates a new goal for the leader.
    void generateGoal(int i);

    /// Calculates the neighbors of a given bird and optimizes the search by checking if there could be a match for a
    /// given partition. If not, the partition and all its birds are skipped. The returned vector does not include the
    /// argument.
    std::vector<Bird> neighboursOf(const Bird* bird) const;

  public:
    ParAlgorithm() = delete;

    /// All values can not be changed later.
    /// The birds will be copied into a private field.
    /// PartitionOverloadTolerance is a value in percent, e.g. 0.1f means that each partition may hold up
    /// to 10% more birds than what is optimal.
    ParAlgorithm(const std::vector<Bird>& initialBirds, const std::vector<Obstacle>& obstacles, int leaderCount, int width, int height, float visionRadius,
                 int partitionCount, float partitionOverloadTolerance);

    
    struct DrawingInformation {
      const std::vector<Bird>* birds;
      std::vector<float> partitions;
      std::vector<Bird> leaders;
      std::vector<Vec> goals;
      const std::vector<Obstacle>* obstacles;
    };
    DrawingInformation drawingInformation() const;

    virtual ~ParAlgorithm() = default;
    virtual const std::vector<Bird>& update();
};
#endif