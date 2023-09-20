#include "Algorithm.h"
#include <random>

using namespace std;

static constexpr int goalGenerationRetries = 10;

Algorithm::Algorithm(const std::vector<Bird>& initialBirds, const std::vector<Obstacle>& obstacles, int leaderCount,
                     int size, float visionRadius)
    : size(size), visionRadius(visionRadius), birds(initialBirds), obstacles(obstacles)
{
    leaders.resize(leaderCount);
    for (int i = 0; i < leaderCount; i++)
    {
        leaders[i].first = &birds[i];
        generateGoal(i);
    }
}

void Algorithm::generateGoal(int leaderIndex)
{
    random_device dev;
    mt19937 rng(dev());
    uniform_real_distribution<> sizeGen(size / 5.0f * 1, size / 5.0f * 4);

    for (int i = 0; i < goalGenerationRetries; i++)
    {
        leaders[leaderIndex].second.x = sizeGen(rng);
        leaders[leaderIndex].second.y = sizeGen(rng);

        bool tooClose = false;
        for (auto& obs : obstacles)
            if (obs.distanceTo(leaders[leaderIndex].second) < 40)
                tooClose = true;

        if (!tooClose)
            return;
    }
}

std::pair<std::vector<Bird>, std::vector<Obstacle>> Algorithm::objectsInVision(const Bird& bird) const
{
    vector<Bird> bir;
    for (auto& curr : birds)
    {
        if ((curr.position - bird.position).length() < visionRadius)
            bir.push_back(curr);
    }

    vector<Obstacle> obs;
    for (auto& obstacle : obstacles)
    {
        if (obstacle.distanceTo(bird.position) < visionRadius)
            obs.push_back(obstacle);
    }

    return {bir, obs};
}