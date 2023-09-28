#include "Bird.h"
#include "Obstacle.h"
#include <algorithm>
#include <bits/ranges_algo.h>
#include <cmath>
#include <limits>

using namespace std;

constexpr float maxSpeed = 3.0f;
constexpr float minSpeed = 1.5f;
constexpr float maxForce = 30.0f;
constexpr float goalAttractionBreakOne = 100.0f;
constexpr float collisionBreakOne = 400;
constexpr float collisionLinear = 4;
constexpr float collisionRadiusBreakOne = 30;
constexpr float collisionMax = 400;
constexpr float leaderAttractionScale = 0.09;
constexpr float separationBreakOne = 10;
constexpr float alignmentScale = 1.5;
constexpr float cohesionScale = 1.0;
constexpr float overallForceScale = 0.02;
constexpr float keepForceScale = 0.7;

Vec Bird::calculateAlignment(const vector<Bird>& neighbors) const
{
    if (neighbors.empty())
        return {0, 0};

    Vec neighborVelocity;
    for (size_t j = 0; j < neighbors.size(); ++j)
        neighborVelocity += neighbors[j].velocity;

    neighborVelocity = neighborVelocity / neighbors.size();

    return (neighborVelocity - velocity).normalized() * alignmentScale;
}

Vec Bird::calculateSeparationPushBack(const vector<Bird>& neighbors) const
{
    Vec accum;
    for (size_t j = 0; j < neighbors.size(); ++j)
    {
        Vec diff = position - neighbors[j].position;
        Vec seperation = diff.normalized() * separationBreakOne / diff.length();
        accum += seperation;
    }

    return accum;
}

Vec Bird::calculateCohesionPull(const vector<Bird>& neighbors) const
{

    Vec averagePosition;
    if (neighbors.empty())
        return {0, 0};

    for (size_t j = 0; j < neighbors.size(); ++j)
        averagePosition = averagePosition + neighbors[j].position;

    averagePosition = averagePosition / neighbors.size();
    return (averagePosition - position).normalized() * cohesionScale;
}

Vec Bird::calculateCollisionPushBack(const vector<Obstacle>& obstacles) const
{
    Vec accum;
    for (auto& obs : obstacles)
    {
        float dist = obs.distanceTo(position);
        if (dist == 0)
            continue;

        Vec diff = obs.center - position;

        auto linear = diff;
        linear = linear.normalized() * -collisionLinear;

        auto square = diff;
        square.toLength(-collisionBreakOne / dist);
        accum += (square + linear) * obs.radius / collisionRadiusBreakOne;
    }

    accum.limitLength(collisionMax);

    return accum;
}
Vec Bird::calculateLeaderAttraction(const vector<Bird>& leaders) const
{
    auto nearest = ranges::min_element(leaders, {}, [this](auto& b) { return (b.position - position).length(); });
    Vec diff = nearest->position - position;
    diff *= leaderAttractionScale;
    return diff;
}
Vec Bird::calculateGoalAttraction(const Vec& goal) const
{
    Vec diff = goal - position;
    diff.toLength(abs(diff.length() - goalAttractionBreakOne) * 0.5);
    return diff;
}

void Bird::applyForce(const Vec& force)
{
    Vec adjusted = force * keepForceScale;
    adjusted.limitLength(maxForce);
    velocity += adjusted * overallForceScale;
    velocity.limitLength(maxSpeed);
    if (velocity.length() < minSpeed)
        velocity.toLength(minSpeed);
    angle = velocity.angle();
}

void Bird::applyVelocity()
{
    position += velocity;
}
