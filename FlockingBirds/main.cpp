#include <SDL.h>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <iostream>
#include <random>

#include "Obstacle.h"
#include "ParAlgorithm.h"
#include "SimpleAlgorithm.h"
#include "Simulation.h"

#include "Draw.h"
#include <omp.h>
#include <vector>

// more leaders
// obstacles
// smooth angles for display


int main(int argc, char** argv)
{
    omp_set_num_threads(5);
    const int size = 1500;
    initDrawing(size);

    std::vector<Bird> birds;
    std::vector<Obstacle> obstacles;


    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<> pos(0, size);
    std::uniform_real_distribution<> radius(10, 50);

    std::uniform_real_distribution<> angle(0, 2 * 3.141);

    for (int i = 0; i < 1000; i++)
    {
        Bird bird;
        bird.position.x = pos(rng);
        bird.position.y = pos(rng);

        birds.push_back(bird);
    }

     for (int i = 0; i < 15; i++)
    {
        Obstacle obstacle;
        obstacle.center.x = pos(rng);
        obstacle.center.y = pos(rng);
        obstacle.radius = radius(rng);

        obstacles.push_back(obstacle);
    }


    ParAlgorithm alg(birds, obstacles, 3, size, size, 100, 5, .1f);
    // SimpleAlgorithm alg(birds, obstacles);

    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                quit = true;
        }
        // SDL_Delay(100);
        alg.update();
        drawParallel(alg.drawingInformation());
    }

    SDL_Quit();
    return 0;
}