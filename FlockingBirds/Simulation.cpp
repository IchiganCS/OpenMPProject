#include "Simulation.h"
#include "Bird.h"
#include "Draw.h"
#include "render/MyRenderer.h"
#include "utils/Utils.hpp"
#include <string>

int count = 0;

void Simulation::simulate() {
  MyRenderer r(std::string("abcd"));
  while (true) {
    Algo(Birds);
    Utils::intialiseInitPostion(Birds, 20);
    r.update(Birds);
    Utils::printBirdsData(Birds);
    draw(Birds);
  }
}

Simulation::Simulation(Algorithm a, int count) {
  Algo = a;
  Utils::intialiseInitPostion(Birds, count);
}