#ifndef BIRD_H
#define BIRD_H


#include "Vec.h"
#include <vector>

class Bird {
    public:
    Vec position;
    Vec force;
    float angle;

    void addLeaderAttraction(const Vec& leaderPosition);
    void addGoalAttraction(const Vec& goalPosition);
    void addSeparationPushBack(const Vec& neighborPosition);
    void addCohesionPull(const std::vector<Bird*>& neighbors);
    void addCollisionPushBack(const Vec& obstaclePosition);
    void addAlignment(const std::vector<Bird*>& neighbors);
    void applyForce();


    bool operator==(const Bird& other) const = default;
    bool operator!=(const Bird& other) const = default;
};

#endif