#include "Simulation.h"
#include "Bird.h"
#include "Draw.h"

void Simulation::simulate() {
  while (true) {
    Birds = Algo->update(0.05f);

    // drawBirds(Birds);
  }
}

Simulation::Simulation(std::unique_ptr<Algorithm> &&a, int count) {
  Algo = std::move(a);
  Birds.reserve(count);
}