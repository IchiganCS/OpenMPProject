#include "ParallelForAlgo.h"
#include "ParallelForAlgoWithRed.h"
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

//   Utils::csv.writePoint(0, 0, 0, 0, 0, 0, 0);
// #ifndef EXCLUDESDL

//   int w, h;
//   w = dm.w;
//   h = dm.h;
//   Utils::WINDOW_HEIGHT = 0.8 * dm.h;
//   Utils::WINDOW_WIDTH = 0.8 * dm.w;
// #endif
//   SimpleFlockingBird simpleFlockingBird;
//   ParallelForAlgo parallelForAlgo;
//   ParallelForAlgoWithRed parallelForAlgoWithRed;
//   std::vector<Bird> birds;
//   std::vector<Obstacle> obstacles;
// #ifndef EXCLUDESDL

//   initDrawing(Utils::WINDOW_HEIGHT);
// #endif
//   Utils::intialiseInitPostion(birds, 100);
//   Utils::intialiseInitObstacles(obstacles, 5);

//   std::cout << "Running" << std::endl;
//   // Simulation s(&simpleFlockingBird, birds, obstacles);
//   Simulation s(&simpleFlockingBird, &parallelForAlgo,
//   &parallelForAlgoWithRed,
//                birds, obstacles);

//   s.simulate();
// #ifndef EXCLUDESDL

//   SDL_Quit();
// #endif
//   return 0;
// }

int main() {
  Utils::PRINTLOG = false;
  Utils::csv.writePoint(0, 0, 0, 0, 0, 0, 0);
  int maxThreads = omp_get_max_threads();
  std::vector<Bird> birds;
  std::vector<Obstacle> obstacles;
  int maxBird = 1000;
  int MaxObs = 100;
  int maxIterations = 10;
  SimpleFlockingBird algo;
  float total = maxThreads * (maxBird / 10) * (MaxObs / 10) * maxIterations;
  int count = 0;
  for (int noOfThreads = 1; noOfThreads <= maxThreads; noOfThreads++) {
    for (int noOfBirds = 1; noOfBirds <= maxBird; noOfBirds += 10) {
      for (int noOfOBS = 1; noOfOBS <= MaxObs; noOfOBS += 10) {
        Utils::NUM_THREADS = noOfThreads;
        Utils::intialiseInitPostion(birds, noOfBirds);
        Utils::intialiseInitObstacles(obstacles, noOfOBS);
        for (int noOfIterations = 1; noOfIterations <= maxIterations;
             noOfIterations++) {
          count++;
          // assert(count == noOfThreads * noOfBirds * noOfOBS *
          // noOfIterations);
          std::cout << "Test No: " << count << " of total: " << int(total)
                    << " done. Percent: " << ((count / total) * 100.0f)
                    << "%\n";
          algo.update(birds, obstacles);
        }
      }
    }
  }
}