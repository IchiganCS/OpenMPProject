#include "Simulation.h"
#include "Bird.h"
#include "Draw.h"
#include "src/utils/Utils.h"
#include <SDL2/SDL.h>
#include <string>

int count = 0;

void Simulation::simulate() {
  SDL_Event e;
  bool running = true;
  while (running) {
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
          if (Birds.size() < 1000)
            Birds.push_back(Utils::getBird());
          break;
        }
        case SDLK_b: {
          if (Birds.size() > 1)
            Birds.pop_back();
          break;
        }

        default: { ; }
        }
      }
    }
    if (running) {
      algo->update(Birds);
      drawOnlyBirds(Birds);
      printf("FPS = %d\n", Utils::FPS);
      SDL_Delay((1000 / Utils::FPS));
    }
  }
}

Simulation::Simulation(Algorithm *a, std::vector<Bird> &birds)
    : Birds(birds), algo(a) {
  ;
}