#include <iostream>

#include "Simulation.h"
#include "SeqAlgorithm.h"

#include <SDL.h>



int main(int argc, char** argv) {

    SDL_Init(0);

    std::cout << "Running" << std::endl;
    Simulation s(seqAlgorithm, 200);
    s.simulate();

    return 0;
}