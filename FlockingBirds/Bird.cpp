#include "Bird.h"
#include <algorithm>
#include <cmath>

using namespace std;

constexpr float leaderAttractionScale = 0.05;
constexpr float separationBreakOne = 40.0;
constexpr float cohesionScale = 0.0001;
constexpr float goalAttraction = 15.0;
constexpr float collisionBreakOne = 5.0;
constexpr float alignmentScale = 4.0;
constexpr float maxSpeed = 5.0;

void Bird::addLeaderAttraction(const Vec& leaderPosition)
{
    auto value = leaderPosition - position;
    value.scale(leaderAttractionScale);
    force += value;
}
void Bird::addGoalAttraction(const Vec& goalPosition)
{
    auto goalAttrForce = goalPosition - position;
    goalAttrForce.toLength(min(5000.0f / goalAttrForce.length(), 20.0f));
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

void Bird::addCollisionPushBack(const Vec& obstaclePosition)
{
}
void Bird::addAlignment(const vector<Bird*>& neighbors)
{
    if (neighbors.size() == 0)
        return;

    float neighbourDirection;
    for (auto& neigh : neighbors)
        neighbourDirection += neigh->angle;
    neighbourDirection /= neighbors.size();

    Vec forceAngle = {sin(neighbourDirection), cos(neighbourDirection)};
    forceAngle.scale(alignmentScale);
    force += forceAngle;
}

void Bird::applyForce()
{
    if (force.x == 0 && force.y == 0)
        return;

    force.limitLength(maxSpeed);
    position += force;

    constexpr float angleConservation = 0.4;
    angle *= angleConservation;
    angle += force.angle() * (1 - angleConservation);
    force = {0, 0};
}