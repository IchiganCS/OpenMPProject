#include <iostream>

#include "SeqAlgorithm.h"
#include "Simulation.h"

#include <SDL.h>

int main(int argc, char **argv) {

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL could not be initialized: " << SDL_GetError();
  } else {
    std::cout << "SDL video system is ready to go\n";
  }

  std::cout << "Running" << std::endl;
  Simulation s(seqAlgorithm, 5);
  s.simulate();

  SDL_Quit();
  return 0;
}