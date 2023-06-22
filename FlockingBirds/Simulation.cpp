#include "Simulation.h"
#include "Bird.h"
#include "Draw.h"

void Simulation::simulate() {
    while (true) {
        Algo(Birds);

        draw(Birds);
    }
}

Simulation::Simulation(Algorithm a, int count) {
    Algo = a;
    Birds.reserve(count);
}