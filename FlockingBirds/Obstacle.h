#pragma once

#include "Vec.h"

struct Obstacle
{
    Vec center;
    float radius;

    float distanceTo(const Vec& position) const;
};