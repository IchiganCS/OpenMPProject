#include "SimpleFlockingBird.h"
#include "Utils.h"
#include "src/Draw.h"
#include "src/Simulation.h"
#include <iostream>

#ifdef _OPENMP
#include <omp.h>
#else
#error "OpenMP support required"
#endif
#ifndef EXCLUDESDL

#include <SDL.h>
#endif
// int main(int argc, char **argv) {
// #ifndef EXCLUDESDL

//   if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//     std::cout << "SDL could not be initialized: " << SDL_GetError();
//   } else {
//     std::cout << "SDL video system is ready to go\n";
//   }
//   SDL_DisplayMode dm;

//   if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
//     SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
//   }
// #endif
//   Utils::NUM_THREADS = omp_get_max_threads();

//   Utils::csv.writePoint(0, 0, 0, 0, 0, 0);
// #ifndef EXCLUDESDL

//   int w, h;
//   w = dm.w;
//   h = dm.h;
//   Utils::WINDOW_HEIGHT = 0.8 * dm.h;
//   Utils::WINDOW_WIDTH = 0.8 * dm.w;
// #endif
//   SimpleFlockingBird algo;
//   std::vector<Bird> birds;
//   std::vector<Obstacle> obstacles;
// #ifndef EXCLUDESDL

//   initDrawing(Utils::WINDOW_HEIGHT);
// #endif
//   Utils::intialiseInitPostion(birds, 1);
//   Utils::intialiseInitObstacles(obstacles, 1);

//   std::cout << "Running" << std::endl;
//   Simulation s(&algo, birds, obstacles);
//   s.simulate();
// #ifndef EXCLUDESDL

//   SDL_Quit();
// #endif
//   return 0;
// }

int main() {
  Utils::csv.writePoint(0, 0, 0, 0, 0, 0, 0);
  int maxThreads = omp_get_max_threads();
  std::vector<Bird> birds;
  std::vector<Obstacle> obstacles;
  int maxBird = 1000;
  int MaxObs = 100;
  int maxIterations = 10;
  SimpleFlockingBird algo;
  for (int noOfThreads = 8; noOfThreads <= maxThreads; noOfThreads++) {
    for (int noOfBirds = 0; noOfBirds <= maxBird; noOfBirds += 10) {
      for (int noOfOBS = 0; noOfOBS <= MaxObs; noOfOBS += 10) {
        Utils::NUM_THREADS = noOfThreads;
        Utils::intialiseInitPostion(birds, noOfBirds);
        Utils::intialiseInitObstacles(obstacles, noOfOBS);
        for (int noOfIterations = 0; noOfIterations < maxIterations;
             noOfIterations++) {
          algo.update(birds, obstacles);
        }
      }
    }
  }
}