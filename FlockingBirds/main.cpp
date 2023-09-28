#include <SDL.h>
#include <SDL_events.h>
#include <SDL_keycode.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <fstream>
#include <iostream>
#include <random>

#include "Benchmark.h"
#include "CSVWriter.h"
#include "Obstacle.h"
#include "ParAlgorithm.h"
#include "SimpleAlgorithm.h"

#include "Draw.h"
#include <omp.h>
#include <vector>

std::pair<std::vector<Bird>, std::vector<Obstacle>> ranGen(int size, int birdCount, int obstacleCount)
{
    std::vector<Bird> birds;
    std::vector<Obstacle> obstacles;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<> pos(0, size);
    std::uniform_real_distribution<> radius(10, 50);

    std::uniform_real_distribution<> angle(0, 2 * std::numbers::pi);

    for (int i = 0; i < birdCount; i++)
    {
        Bird bird;
        bird.position.x = pos(rng);
        bird.position.y = pos(rng);

        birds.push_back(bird);
    }

    for (int i = 0; i < obstacleCount; i++)
    {
        Obstacle obstacle;
        obstacle.center.x = pos(rng);
        obstacle.center.y = pos(rng);
        obstacle.radius = radius(rng);

        obstacles.push_back(obstacle);
    }

    return {birds, obstacles};
}

void benchmarkAll()
{
    std::ofstream file;
    file.open("hardwareTest.csv");

    writeHeader(file);

    // std::vector<int> birdCounts = {200, 500, 1000};
    std::vector<int> birdCounts = {5000};
    std::vector<int> sizes = {300 };
    // std::vector<int> sizes = {300, 1000, 1500};
    std::vector<int> radiuses = {100};
    // std::vector<int> radiuses = {10, 100};
    std::vector<int> obstacleCounts = { 40};
    // std::vector<int> obstacleCounts = {10, 40};
    std::vector<int> leaderCounts = {3};
    // std::vector<int> threadCounts = {2, 20};
    std::vector<int> threadCounts = { 16 };
    // std::vector<int> threadCounts = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
    std::vector<float> partitionOverloads = {0.3f};
    // std::vector<float> partitionOverloads = {0.1f, 0.3f, 0.6f, 0.8f, 1.0f};

    for (auto birdCount : birdCounts)
        for (auto size : sizes)
            for (auto radius : radiuses)
                for (auto leaderCount : leaderCounts)
                    for (auto obstacleCount : obstacleCounts)
                    {
                        auto [birds, obstacles] = ranGen(size, birdCount, obstacleCount);

                        SimpleAlgorithm alg(birds, obstacles, leaderCount, size, radius);
                        // benchmarkAndWriteMany(&alg, file, 500, size, radius, leaderCount);

                        for (auto threadCount : threadCounts)
                        {
                            for (auto partitionOverload : partitionOverloads)
                            {
                                ParAlgorithm alg(birds, obstacles, leaderCount, size, radius, threadCount,
                                                 partitionOverload);
                                benchmarkAndWriteMany(&alg, file, 50000, size, radius, leaderCount);
                            }
                        }
                    }

    file.close();
}

int main(int argc, char** argv)
{
    bool benchmark = true;

    if (benchmark)
    {
        benchmarkAll();
        return 0;
    }
    else {
        const int size = 1500;
        initDrawing(size);
        auto [birds, obstacles] = ranGen(size, 1000, 30);

        ParAlgorithm alg(birds, obstacles, 3, size, 300, 5, 0.1f);

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
}