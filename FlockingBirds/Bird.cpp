#include "Bird.h"
#include "Obstacle.h"
#include <algorithm>
#include <bits/ranges_algo.h>
#include <cmath>
#include <limits>

using namespace std;

constexpr float leaderAttractionScale = 0.1;
constexpr float separationBreakOne = 35.0;
constexpr float cohesionScale = 0.0001;
constexpr float goalAttraction = 100000.0;
constexpr float collisionBreakOne = 1000.0;
constexpr float collisionCutOff = 200.0;
constexpr float alignmentScale = 15.0;
constexpr float maxSpeed = 5.0;
constexpr float angleConservation = 0.4;
constexpr float overallScale = 0.5;

void Bird::addLeaderAttraction(const vector<Bird>& leaders)
{
    auto nearest = ranges::min_element(leaders, {}, [this](auto& b) { return (b.position - position).length(); });
    Vec value = nearest.base()->position - this->position;
    value.scale(leaderAttractionScale);

    force += value;
}

void Bird::addGoalAttraction(const Vec& goalPosition)
{
    auto goalAttrForce = goalPosition - position;
    goalAttrForce.toLength(min(goalAttraction / goalAttrForce.length(), 50.0f));
    force += goalAttrForce;
}

void Bird::addSeparationPushBack(const Vec& neighborPosition)
{
    Vec dist = neighborPosition - position;
    float distLength = dist.length();

    // to avoid dividing by zero, this check is put in place
    // of course, if the distance were to be zero, it should acutally
    // have an infinite push back force
    // it is assumed that this is inconsistency is resolved in the next iteration
    // through other means
    if (distLength == 0)
        return;

    dist.toLength(-separationBreakOne / distLength);
    force += dist;
}

void Bird::addCohesionPull(const std::vector<Bird*>& neighbors)
{
    if (neighbors.size() == 0)
        return;

    Vec neighbourPosition;
    for (auto& neigh : neighbors)
        neighbourPosition += neigh->position;
    neighbourPosition.scale(1.0f / neighbors.size());

    auto cohesion = neighbourPosition - position;
    cohesion.scale(cohesionScale);
    force += cohesion;
}

void Bird::addCollisionPushBack(const Obstacle& obstacle)
{
    Vec dir = obstacle.center - position;
    float distLength = obstacle.distanceTo(position);

    if(distLength != 0 && distLength < collisionCutOff) {
        dir.toLength(-collisionBreakOne / distLength);
        force += dir;
    }
}

void Bird::addAlignment(const vector<Bird*>& neighbors)
{
    if (neighbors.size() == 0)
        return;

    float neighbourDirection;
    Vec avgNeighDist;
    for (auto& neigh : neighbors) {
        Vec neighDist = neigh->position - position;
        avgNeighDist += neighDist;
        neighbourDirection += neigh->angle;
    }
    neighbourDirection /= neighbors.size();

    Vec forceAngle = {sin(neighbourDirection), cos(neighbourDirection)};
    forceAngle.scale(alignmentScale);
    force += forceAngle;
}

void Bird::applyForce()
{
    if (force.x == 0 && force.y == 0)
        return;

    force.scale(overallScale);
    force.limitLength(overallScale * maxSpeed);
    position += force;

    angle *= angleConservation;
    angle += force.angle() * (1 - angleConservation);
    force = {0, 0};
}