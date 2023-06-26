#include "src/SimpleFlockingBird/SimpleFlockingBird.h"
#include "src/Simulation.h"
#include "src/utils/Utils.h"
#include <iostream>

#include <SDL.h>

int main(int argc, char **argv) {

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL could not be initialized: " << SDL_GetError();
  } else {
    std::cout << "SDL video system is ready to go\n";
  }
  SimpleFlockingBird algo;
  std::vector<Bird> birds;

  Utils::intialiseInitPostion(birds, 20);

  std::cout << "Running" << std::endl;
  Simulation s(&algo, birds);
  s.simulate();

  SDL_Quit();
  return 0;
}