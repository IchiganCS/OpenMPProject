#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Bird.h"
#include <vector>


class Algorithm {
    public:
    virtual ~Algorithm() = default;
    virtual const std::vector<Bird>& update(float delta) = 0;
};

#endif