#ifndef UTILS_H
#define UTILS_H

#include "Bird.h"
#include "Vec.h"

Vec getAlignment(const std::vector<Bird>& neighbors, const Bird& current);
Vec getSeparation(const std::vector<Bird>& neighbors, const Bird& current);
Vec getCohesion(const std::vector<Bird>& neighbors, const Bird& current);
Vec getAvoidance(const std::vector<Obstacle>& obstacles, const Bird& current);
Vec getLeaderAttraction(const std::vector<Bird>& leaders, const Bird& current);
Vec getGoalAttraction(const Vec& goal, const Bird& leader);
Vec combineForces(const Vec& cohesion, const Vec& alignment, const Vec& separation, const Vec& avoidance,
                  const Vec& goalAttraction, const Vec& leaderAttraction);
void addForce(const Vec& force, Bird& bird);

#endif