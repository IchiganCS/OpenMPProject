#include "ParAlgorithm.h"
#include <algorithm>
#include <bits/ranges_algo.h>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

ParAlgorithm::ParAlgorithm(const std::vector<Bird>& initialBirds, int width, int height, float visionRadius,
                           float maxSpeed, int partitionCount)
    : width(width), height(height), visionRadius(visionRadius), maxSpeed(maxSpeed), birds(initialBirds)
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
    std::uniform_real_distribution<> widthGen(0, width);
    std::uniform_real_distribution<> heightGen(0, height);

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

        min = partIdx == 0 ? 0 : get<2>(partitions[partIdx - 1]);
        max = partIdx == partitions.size() - 1 ? width : currentPart.back()->position.x;

        handledBirds += roundDown;
        birdsForCurrentPartition = (birdsForCurrentPartition - roundDown) + birdsPerPartition;
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
        if (bird->position.x > min && bird->position.x < max)
        {
            partBirds.push_back(bird);
            break;
        }
    }
}

const std::vector<Bird>& ParAlgorithm::update(float delta)
{
    if ((leader->position - leaderGoal).length() < 5)
        generateGoal();

    for (auto& bird : birds)
    {
        bird.force = Vec();

        auto leaderAttraction = leader->position - bird.position;
        leaderAttraction.scale(leaderAttractionScale);
        bird.force += leaderAttraction;

        auto neighbours = neighboursOf(&bird);

        Vec neighbourPosition;
        for (auto& neigh : neighbours)
            neighbourPosition += neigh->position;
        neighbourPosition.scale(1.0f / neighbours.size());

        auto cohesion = neighbourPosition - bird.position;
        cohesion.scale(cohesionScale);
        bird.force += cohesion;

        Vec separation;
        for (auto& neigh : neighbours)
        {
            Vec dist = neigh->position - bird.position;
            float distLength = dist.length();
            dist.toLength(-separationBreakOne / distLength);
            separation += dist;
        }
        bird.force += separation;
    }

    auto goalAttrForce = leaderGoal - leader->position;
    goalAttrForce.toLength(goalAttraction);
    leader->force += goalAttrForce;

    for (auto& [partBirds, min, max] : partitions)
    {
        for (int i = 0; i < partBirds.size(); i++)
        {
            Bird* bird = partBirds[i];
            bird->force.limitLength(maxSpeed);
            bird->position += bird->force;
            bird->angle = bird->force.angle();
            bird->force = Vec();

            if (bird->position.x < min || bird->position.x > max)
            {
                partBirds.erase(partBirds.begin() + i);
                reassignPartition(bird);
            }
        }
    }

    return birds;
}