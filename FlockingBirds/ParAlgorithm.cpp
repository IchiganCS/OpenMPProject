#include "ParAlgorithm.h"

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

ParAlgorithm::ParAlgorithm(const std::vector<Bird>& initialBirds, int width, int height, float visionRadius,
                           int partitionCount, float partitionOverloadTolerance)
    : width(width), height(height), visionRadius(visionRadius), birds(initialBirds),
      partitionMaxElements((1 + partitionOverloadTolerance) * initialBirds.size() / partitionCount),
      partitionOverloadTolerance(partitionOverloadTolerance)
{
    leader = &birds[0];
    generateGoal();

    partitions.resize(partitionCount);
    repartition();
}

void ParAlgorithm::generateGoal()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<> widthGen(width / 3.0f * 1, width / 3.0f * 2);
    std::uniform_real_distribution<> heightGen(height / 3.0f * 1, height / 3.0f * 2);

    leaderGoal.x = widthGen(rng);
    leaderGoal.y = heightGen(rng);
}

void ParAlgorithm::repartition()
{
    // sort by x
    Bird leaderData = *leader;
    ranges::sort(birds, {}, [](const Bird& bird) { return bird.position.x; });
    for (auto& bird : birds)
    {
        if (bird == leaderData)
        {
            leader = &bird;
            break;
        }
    }

    const float birdsPerPartition = (float)birds.size() / partitions.size();
    float birdsForCurrentPartition = birdsPerPartition;
    int handledBirds = 0;

    for (int partIdx = 0; partIdx < partitions.size(); partIdx++)
    {
        auto& [currentPart, min, max] = partitions[partIdx];
        int roundDown = std::floor(birdsForCurrentPartition);
        currentPart.clear();
        currentPart.reserve(roundDown);

        for (int i = 0; i < roundDown; i++)
            currentPart.push_back(&birds[handledBirds + i]);

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

vector<Bird*> ParAlgorithm::neighboursOf(const Bird* bird) const
{
    // calculate neighbouring partitions in radius vision
    vector<int> partitionIndices;
    for (int i = 0; i < partitions.size(); i++)
    {
        if (bird->position.x + visionRadius > get<1>(partitions[i]) &&
            bird->position.x - visionRadius < get<2>(partitions[i]))
            partitionIndices.push_back(i);
    }

    vector<Bird*> res;
    for (auto partIdx : partitionIndices)
    {
        for (auto& other : get<0>(partitions[partIdx]))
            if (other != bird && other->position.distanceTo(bird->position) < visionRadius)
                res.push_back(other);
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

void ParAlgorithm::applyAllForces()
{
    vector<Bird*> toRepartition;
    for (int partIdx = 0; partIdx < partitions.size(); partIdx++)
    {
        auto& [partBirds, min, max] = partitions[0];

        for (int i = 0; i < partBirds.size(); i++)
        {
            Bird* bird = partBirds[i];
            bird->applyForce();
            if (bird->position.x < min || bird->position.x > max)
            {
                partBirds.erase(partBirds.begin() + i);
                toRepartition.push_back(bird);
                i--;
            }
        }

        for (int i = 0; i < toRepartition.size(); i++)
        {
            Bird* bird = toRepartition[i];
            if (bird->position.x > min && bird->position.x < max)
            {
                toRepartition.erase(toRepartition.begin() + i);
                partBirds.push_back(bird);
                i--;
            }
        }
    }

    // for (auto&)
    //     reassignPartition(toRepartition);
}

const std::vector<Bird>& ParAlgorithm::update(float delta)
{
    if ((leader->position - leaderGoal).length() < 5)
        generateGoal();

    for (auto& bird : birds)
    {
        bird.force = {0, 0};

        bird.addLeaderAttraction(leader->position);

        auto neighbours = neighboursOf(&bird);
        if (neighbours.size() == 0)
            continue;

        bird.addCohesionPull(neighbours);
        bird.addAlignment(neighbours);

        for (auto& neigh : neighbours)
            bird.addSeparationPushBack(neigh->position);
    }
    leader->addGoalAttraction(leaderGoal);

    // applyAllForces();

    // the following block should
    for (auto& [partBirds, min, max] : partitions)
    {
        for (int i = 0; i < partBirds.size(); i++)
        {
            Bird* bird = partBirds[i];
            bird->applyForce();

            if (bird->position.x < min || bird->position.x > max)
            {
                partBirds.erase(partBirds.begin() + i);
                i--;
                reassignPartition(bird);
            }
        }
    }

    repartitionIfOverloaded();

    return birds;
}

tuple<vector<Bird>, vector<float>, Vec, Bird> ParAlgorithm::drawingInformation()
{
    vector<float> parititionLines;

    parititionLines.push_back(get<1>(partitions[0]));

    for (auto& [partBirds, min, max] : partitions)
        parititionLines.push_back(max);

    return make_tuple(birds, parititionLines, leaderGoal, *leader);
}