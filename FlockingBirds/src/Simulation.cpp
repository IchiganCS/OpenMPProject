#include "Simulation.h"
#include "Bird.h"
#include "Draw.h"
#include "src/render/MyRenderer.h"
#include "src/utils/Utils.h"
#include <string>

int count = 0;

void Simulation::simulate() {
  MyRenderer r(std::string("abcd"));
  while (true) {
    algo->update(Birds);
    r.update(Birds);
    draw(Birds);
  }
}

Simulation::Simulation(Algorithm *a, std::vector<Bird> &birds)
    : Birds(birds), algo(a) {
  ;
}