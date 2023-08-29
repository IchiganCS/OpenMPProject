#include "SimpleAlgorithm.h"
#include "Algorithm.h"

using namespace std;

SimpleAlgorithm::SimpleAlgorithm(const std::vector<Bird>& initialBirds, const std::vector<Obstacle>& obstacles,
                                 int leaderCount, int size, float visionRadius)
    : Algorithm(initialBirds, obstacles, leaderCount, size, visionRadius)
{
}

void SimpleAlgorithm::update()
{
    vector<Bird> leaderVals;
    for (auto& [leader, goal] : leaders)
        leaderVals.push_back(*leader);

    vector<Vec> forcePerBird;
    forcePerBird.resize(birds.size());

    for (int i = 0; i < birds.size(); i++)
    {
        auto& bird = birds[i];
        Vec& currentForce = forcePerBird[i];
        currentForce += bird.calculateLeaderAttraction(leaderVals);

        auto [neighbours, seenObstacles] = objectsInVision(bird);
        if (neighbours.size() == 0)
            continue;

        currentForce += bird.calculateCohesionPull(neighbours);
        currentForce += bird.calculateAlignment(neighbours);
        currentForce += bird.calculateSeparationPushBack(neighbours);
        currentForce += bird.calculateCollisionPushBack(seenObstacles);
    }

    for (int i = 0; i < leaders.size(); i++)
    {
        auto& [leader, goal] = leaders[i];

        if ((leader->position - goal).length() < 13)
            generateGoal(i);

        forcePerBird[leader - birds.begin().base()] += leader->calculateGoalAttraction(goal);
    }

    for (int i = 0; i < birds.size(); i++)
    {
        auto bird = birds[i];
        Vec& currentForce = forcePerBird[i];
        bird.applyForce(currentForce);
        bird.applyVelocity();
        currentForce = {0, 0};
    }
}

void SimpleAlgorithm::fillCSV(CSVEntry& entry)
{
    entry.methodName = "simple";
    entry.birdCount = birds.size();
    entry.obstacleCount = obstacles.size();
    entry.partitionOverload = {};
    entry.threadCount = {};
}