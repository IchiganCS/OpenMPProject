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
constexpr float goalAttractionBreakOne = 70.0f;
constexpr float collisionBreakOne = 300;
constexpr float leaderAttractionScale = 0.025;
constexpr float separationScale = 5;
constexpr float alignmentScale = 1.5;
constexpr float cohesionScale = 1.0;
constexpr float overallForceScale = 0.02;

Vec Bird::calculateAlignment(const vector<Bird>& neighbors) const
{
    Vec average_align_velocity;
    int align_neighbours_count = 0;
    for (size_t j = 0; j < neighbors.size(); ++j)
    {
        Vec diff = position - neighbors[j].position;
        align_neighbours_count++;
        average_align_velocity += neighbors[j].velocity;
    }

    if (align_neighbours_count == 0)
        return {0, 0};

    average_align_velocity = average_align_velocity / align_neighbours_count;
    return (average_align_velocity - velocity).normalized() * alignmentScale;
}

Vec Bird::calculateSeparationPushBack(const vector<Bird>& neighbors) const
{
    Vec sep_vec;
    for (size_t j = 0; j < neighbors.size(); ++j)
    {
        Vec diff = position - neighbors[j].position;
        double distance = diff.length();


        sep_vec += diff.normalized() * (distance);
    }

    return sep_vec.normalized() * separationScale;
}

Vec Bird::calculateCohesionPull(const vector<Bird>& neighbors) const
{

    Vec average_position;
    int align_neighbours_count = 0;

    for (size_t j = 0; j < neighbors.size(); ++j)
    {
        if (neighbors[j] == *this)
            continue;

        Vec diff = position - neighbors[j].position;

        align_neighbours_count++;
        average_position = average_position + neighbors[j].position;
    }

    if (align_neighbours_count == 0)
        return {0, 0};
        

    average_position = average_position / align_neighbours_count;
    return (average_position - position).normalized() * cohesionScale;
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

        diff.toLength(-collisionBreakOne / dist);
        accum += diff;
    }

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
    Vec adjusted = force;
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
