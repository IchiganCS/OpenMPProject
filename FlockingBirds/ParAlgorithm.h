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

    /// Applies the forces stored in each bird ot themselves and 
    /// corrects the partitioning if necessary.
    void applyAllForces();

    /// A vector of partitions.
    /// Each partition stores a vector of pointer to birds (pointer into the "birds" vector) as well as a minimal x and
    /// a maximal x for which it is responsible.
    std::vector<std::tuple<std::vector<Bird*>, float, float>> partitions;

    /// Holds all birds. Each modification to the vector itself has to be done carefully:
    /// There are numerous pointer into the vector and each operation which reallocates or shuffles the elements
    /// invalidates all pointers.
    std::vector<Bird> birds;

    /// Stores the leader bird - it is a pointer into the birds vector.
    Bird* leader;

    /// The current goal of the leader bird.
    Vec leaderGoal;

    /// Generates a new goal for the leader.
    void generateGoal();

    /// Calculates the neighbors of a given bird and optimizes the search by checking if there could be a match for a
    /// given partition. If not, the partition and all its birds are skipped. The returned vector does not include the
    /// argument.
    std::vector<Bird*> neighboursOf(const Bird* bird) const;

  public:
    ParAlgorithm() = delete;

    /// All values can not be changed later.
    /// The birds will be copied into a private field.
    /// PartitionOverloadTolerance is a value in percent, e.g. 0.1f means that each partition may hold up
    /// to 10% more birds than what is optimal.
    ParAlgorithm(const std::vector<Bird>& initialBirds, int width, int height, float visionRadius, int partitionCount,
                 float partitionOverloadTolerance);

    /// Returns information useful for drawing.
    /// In order: all birds, partition lines, goal, leader bird
    std::tuple<std::vector<Bird>, std::vector<float>, Vec, Bird> drawingInformation();

    virtual ~ParAlgorithm() = default;
    virtual const std::vector<Bird>& update(float delta);
};
#endif