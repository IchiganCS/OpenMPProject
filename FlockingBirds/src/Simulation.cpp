#include "Simulation.h"
#include "Bird.h"
#include "Draw.h"
#include "Utils.h"
#ifndef EXCLUDESDL
#include <SDL2/SDL.h>
#endif
#include <string>

int count = 0;
void Simulation::simulate() {
  // unsigned int turn = 0;
  bool running = true;
#ifndef EXCLUDESDL
  SDL_Event e;
#endif
  while (running) {
#ifndef EXCLUDESDL

    while (SDL_PollEvent(&e)) {

      if (e.type == SDL_QUIT) {
        running = false;
        break;
      } else if (e.type == SDL_KEYDOWN) {

        switch (e.key.keysym.sym) {
        case SDLK_a: {
          if (Utils::ALIGNMENT_FACTOR < 100)
            Utils::ALIGNMENT_FACTOR++;
          break;
        }
        case SDLK_z: {
          if (Utils::ALIGNMENT_FACTOR > 1)
            Utils::ALIGNMENT_FACTOR--;
          break;
        }
        case SDLK_s: {
          if (Utils::SEPRATION_FACTOR < 100)
            Utils::SEPRATION_FACTOR++;
          break;
        }
        case SDLK_x: {
          if (Utils::SEPRATION_FACTOR > 1)
            Utils::SEPRATION_FACTOR--;
          break;
        }
        case SDLK_d: {
          if (Utils::COHESION_FACTOR < 100)
            Utils::COHESION_FACTOR++;
          break;
        }
        case SDLK_c: {
          if (Utils::COHESION_FACTOR > 1)
            Utils::COHESION_FACTOR--;
          break;
        }
        case SDLK_f: {
          if (Utils::FPS < 1000)
            Utils::FPS++;
          break;
        }
        case SDLK_v: {
          if (Utils::FPS > 1)
            Utils::FPS--;
          break;
        }
        case SDLK_g: {
          if (Birds1.size() < 1000)
            Birds1.push_back(Utils::getBird());
          break;
        }
        case SDLK_b: {
          if (Birds1.size() > 1)
            Birds1.pop_back();
          break;
        }
        case SDLK_h: {
          if (obstacles.size() < 1000)
            obstacles.push_back(Utils::getObstacle());
          break;
        }
        case SDLK_n: {
          if (obstacles.size() > 0)
            obstacles.pop_back();
          break;
        }
        case SDLK_j: {
          if (Utils::AVOIDANCE_FACTOR < 100)
            Utils::AVOIDANCE_FACTOR++;
          break;
        }
        case SDLK_m: {
          if (Utils::AVOIDANCE_FACTOR > 1)
            Utils::AVOIDANCE_FACTOR--;
          break;
        }

        default: { ; }
        }
      }
    }
#endif
    if (running) {
      algo1->update(Birds1, obstacles);
      // algo2->update(Birds2, obstacles);
      // algo3->update(Birds3, obstacles);
#ifndef EXCLUDESDL
      Utils::assertBirdDataEqual(Birds1, Birds2);
      Utils::assertBirdDataEqual(Birds1, Birds3);
      if (turn == 0)
        drawOnlyBirds(Birds1, obstacles, birdColor1, true);
      if (turn == 1)
        drawOnlyBirds(Birds2, obstacles, birdColor2, true);
      if (turn == 2)
        drawOnlyBirds(Birds3, obstacles, birdColor3, true);
      printf("FPS = %d\n", Utils::FPS);
      SDL_Delay((1000 / Utils::FPS));
      turn++;
      if (turn > 2)
        turn = 0;
#endif
    }
  }
}
Simulation::Simulation(Algorithm *a1, Algorithm *a2, Algorithm *a3,
                       std::vector<Bird> &birds, std::vector<Obstacle> &o)
    : Birds1(birds), Birds2(birds), Birds3(birds), algo1(a1), algo2(a2),
      algo3(a3), obstacles(o) {
  ;
}
// Simulation::Simulation(Algorithm *a, std::vector<Bird> &birds,
//                        std::vector<Obstacle> &o)
//     : Birds1(birds), algo1(a), obstacles(o) {
//   ;
//}