#pragma once

#include "Obstacle.h"
#include "Vec.h"
#include <vector>

class Bird {
    public:
    Vec position;
    Vec velocity;
    float angle;

    Vec calculateLeaderAttraction(const std::vector<Bird>& leaders) const;
    Vec calculateGoalAttraction(const Vec& goalPosition) const;
    Vec calculateSeparationPushBack(const std::vector<Bird>& neighbors) const;
    Vec calculateCohesionPull(const std::vector<Bird>& neighbors) const;
    Vec calculateCollisionPushBack(const std::vector<Obstacle>& obstacles) const;
    Vec calculateAlignment(const std::vector<Bird>& neighbors) const;
    void applyForce(Vec const& force);
    void applyVelocity();

    bool operator==(const Bird& other) const = default;
    bool operator!=(const Bird& other) const = default;
};