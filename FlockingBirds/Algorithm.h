#pragma once

#include "Bird.h"
#include <vector>


class Algorithm {
    public:
    virtual ~Algorithm() = default;
    virtual const std::vector<Bird>& update() = 0;
};