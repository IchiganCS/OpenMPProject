#include <SDL.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <iostream>
#include <random>

#include "Simulation.h"
#include "SeqAlgorithm.h"


#include "Draw.h"


int main(int argc, char** argv) {

    init();

    std::vector<Bird> birds;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<> pos(0, 800);
    std::uniform_real_distribution<> angle(0, 2 * 3.141);

    for (int i = 0; i < 200; i++) {
        Bird bird;
        bird.position = { static_cast<float>(pos(rng)), static_cast<float>(pos(rng)) };
        bird.angle = angle(rng);

        birds.push_back(bird);
    }

    draw(birds);

    SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = true; } }

    return 0;
}