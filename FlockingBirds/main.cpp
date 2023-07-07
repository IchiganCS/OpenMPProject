#include <SDL.h>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <iostream>
#include <random>

#include "ParAlgorithm.h"
#include "Simulation.h"

#include "Draw.h"

int main(int argc, char** argv)
{

    const int size = 1500;
    initDrawing(size);

    std::vector<Bird> birds;


    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<> pos(0, size);
    std::uniform_real_distribution<> angle(0, 2 * 3.141);

    for (int i = 0; i < 2000; i++)
    {
        Bird bird;
        bird.position.x = pos(rng);
        bird.position.y = pos(rng);

        birds.push_back(bird);
    }

    ParAlgorithm par(birds, size, size, 100, 5, .1f);

    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                quit = true;
        }
        SDL_Delay(100);
        par.update(0.0f);
        drawParallel(par.drawingInformation());
    }

    SDL_Quit();
    return 0;
}