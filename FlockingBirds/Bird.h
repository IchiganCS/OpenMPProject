#ifndef BIRD_H
#define BIRD_H


#include "Vec.h"

class Bird {
    public:
    Vec position;
    Vec force;
    float angle;

    bool operator==(const Bird& other) const = default;
    bool operator!=(const Bird& other) const = default;
};

#endif