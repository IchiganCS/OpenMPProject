#include "ParAlgorithm.h"
#include "Obstacle.h"

#include <algorithm>
#include <bits/ranges_algo.h>
#include <cmath>
#include <compare>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <ranges>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

ParAlgorithm::ParAlgorithm(const std::vector<Bird>& initialBirds, const std::vector<Obstacle>& obstacles,
                           int leaderCount, int width, int height, float visionRadius, int partitionCount,
                           float partitionOverloadTolerance)
    : width(width), height(height), visionRadius(visionRadius), birds(initialBirds), obstacles(obstacles),
      partitionMaxElements((1 + partitionOverloadTolerance) * initialBirds.size() / partitionCount),
      partitionOverloadTolerance(partitionOverloadTolerance)
{
    leaders.resize(leaderCount);
    for (int i = 0; i < leaderCount; i++)
    {
        leaders[i].first = &birds[i];
        generateGoal(i);
    }

    partitions.resize(partitionCount);
    repartition();
}

void ParAlgorithm::generateGoal(int i)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<> widthGen(width / 5.0f * 1, width / 5.0f * 3);
    std::uniform_real_distribution<> heightGen(height / 5.0f * 1, height / 5.0f * 3);

    leaders[i].second.x = widthGen(rng);
    leaders[i].second.y = heightGen(rng);

    for (auto& obs : obstacles)
        if (obs.distanceTo(leaders[i].second) < 50)
        {
            generateGoal(i);
            return;
        }
}

void ParAlgorithm::repartition()
{
    // make a copy of birds which will be sorted by the x-coordinate
    // the second pair entry stores the index in the birds-vector
    vector<pair<Bird*, int>> sorted;
    sorted.reserve(birds.size());
    for (int i = 0; i < birds.size(); i++)
        sorted.push_back(make_pair(&birds[i], i));
    ranges::sort(sorted, {}, [](const auto& bird) { return bird.first->position.x; });

    const float birdsPerPartition = (float)birds.size() / partitions.size();
    float birdsForCurrentPartition = birdsPerPartition;
    int handledBirds = 0;

    for (int partIdx = 0; partIdx < partitions.size(); partIdx++)
    {
        auto& [currentPart, min, max] = partitions[partIdx];
        int roundDown = floor(birdsForCurrentPartition);
        currentPart.clear();
        currentPart.reserve(roundDown);

        for (int i = 0; i < roundDown; i++)
            // take the stored index from sorted as a redirect into the birds array
            currentPart.push_back(&birds[sorted[handledBirds + i].second]);

        min = partIdx == 0 ? -numeric_limits<float>::max() : get<2>(partitions[partIdx - 1]);
        max = partIdx == partitions.size() - 1 ? numeric_limits<float>::max() : currentPart.back()->position.x;

        handledBirds += roundDown;
        birdsForCurrentPartition = (birdsForCurrentPartition - roundDown) + birdsPerPartition;
    }
}

void ParAlgorithm::repartitionIfOverloaded()
{
    for (auto& [partBirds, min, max] : partitions)
    {
        if (partBirds.size() > partitionMaxElements)
        {
            repartition();
            break;
        }
    }
}

vector<Bird> ParAlgorithm::neighboursOf(const Bird* bird) const
{
    // calculate neighbouring partitions in radius vision
    vector<int> partitionIndices;
    for (int i = 0; i < partitions.size(); i++)
    {
        if (bird->position.x + visionRadius > get<1>(partitions[i]) &&
            bird->position.x - visionRadius < get<2>(partitions[i]))
            partitionIndices.push_back(i);
    }

    vector<Bird> res;
    for (auto partIdx : partitionIndices)
    {
        for (auto& other : get<0>(partitions[partIdx]))
            if (other != bird && other->position.distanceTo(bird->position) < visionRadius)
                res.push_back(*other);
    }

    return res;
}

void ParAlgorithm::reassignPartition(Bird* bird)
{
    for (auto& [partBirds, min, max] : partitions)
    {
        if (bird->position.x >= min && bird->position.x <= max)
        {
            partBirds.push_back(bird);
            break;
        }
    }
}

const std::vector<Bird>& ParAlgorithm::update()
{
    vector<Bird> leaderVals;
    for (auto& [leader, goal] : leaders)
        leaderVals.push_back(*leader);

    vector<Vec> forcePerBird;
    forcePerBird.resize(birds.size());

#pragma omp parallel for
    for (int i = 0; i < birds.size(); i++)
    {
        auto& bird = birds[i];
        Vec& currentForce = forcePerBird[i];
        currentForce += bird.calculateLeaderAttraction(leaderVals);

        auto neighbours = neighboursOf(&bird);
        if (neighbours.size() == 0)
            continue;

        currentForce += bird.calculateCohesionPull(neighbours);
        currentForce += bird.calculateAlignment(neighbours);
        currentForce += bird.calculateSeparationPushBack(neighbours);
        currentForce += bird.calculateCollisionPushBack(obstacles);
    }

    for (int i = 0; i < leaders.size(); i++)
    {
        auto& [leader, goal] = leaders[i];

        if ((leader->position - goal).length() < 13)
            generateGoal(i);
        forcePerBird[leader - birds.begin().base()] += leader->calculateGoalAttraction(goal);
    }

    for (auto& [partBirds, min, max] : partitions)
    {
        for (int i = 0; i < partBirds.size(); i++)
        {
            Bird* bird = partBirds[i];
            Vec& currentForce = forcePerBird[bird - birds.begin().base()];
            bird->applyForce(currentForce);
            bird->applyVelocity();
            currentForce = {0, 0};

            if (bird->position.x < min || bird->position.x > max)
            {
                partBirds.erase(partBirds.begin() + i);
                i--;
                // it is possible that a bird may be now moved to a partition which will later be updated again.
                // since applyForce() can be called multiple times and only has a effect once, this is fine
                // but keep it in mind
                reassignPartition(bird);
            }
        }
    }

    repartitionIfOverloaded();

    return birds;
}

ParAlgorithm::DrawingInformation ParAlgorithm::drawingInformation() const
{
    DrawingInformation di;

    vector<float> parititionLines;

    parititionLines.push_back(get<1>(partitions[0]));

    for (auto& [partBirds, min, max] : partitions)
        parititionLines.push_back(max);

    vector<pair<Bird, Vec>> derefLeaders;
    vector<Bird> leaderBirds;
    vector<Vec> leaderGoals;
    for (auto& [leader, goal] : leaders)
    {
        leaderBirds.push_back(*leader);
        leaderGoals.push_back(goal);
    }

    di.partitions = parititionLines;
    di.birds = &birds;
    di.leaders = leaderBirds;
    di.goals = leaderGoals;
    di.obstacles = &obstacles;
    return di;
}