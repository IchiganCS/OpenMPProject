#include "src/Draw.h"
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
  SDL_DisplayMode dm;

  if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
    SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
  }

  int w, h;
  w = dm.w;
  h = dm.h;
  Utils::WINDOW_HEIGHT = 0.8 * dm.h;
  Utils::WINDOW_WIDTH = 0.8 * dm.w;
  SimpleFlockingBird algo;
  std::vector<Bird> birds;
  initDrawing(Utils::WINDOW_HEIGHT);

  Utils::intialiseInitPostion(birds, 1);

  std::cout << "Running" << std::endl;
  Simulation s(&algo, birds);
  s.simulate();

  SDL_Quit();
  return 0;
}