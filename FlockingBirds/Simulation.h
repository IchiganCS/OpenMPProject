#ifndef SIMULATION_H
#define SIMULATION_H

#include <array>
#include "Bird.h"
#include "Algorithm.h"
#include "vector"

class Simulation {
    private:
        std::vector<Bird> Birds;
        Algorithm Algo;
    public:
        void simulate();

        Simulation(Algorithm a, int birdCount);
};

#endif