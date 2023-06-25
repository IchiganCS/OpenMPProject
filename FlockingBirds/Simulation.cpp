#include "Simulation.h"
#include "Bird.h"
#include "Draw.h"
#include "utils/Utils.hpp"
void Simulation::simulate() {
  while (true) {
    Algo(Birds);
    Utils::printBirdsData(Birds);
    draw(Birds);
  }
}

Simulation::Simulation(Algorithm a, int count) {
  Algo = a;
  Utils::intialiseInitPostion(Birds, count);
}