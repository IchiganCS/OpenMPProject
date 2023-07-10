#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Vec.h"
struct Obstacle
{
    Vec center;
    float radius;

    float distanceTo(const Vec& position) const;
};

#endif