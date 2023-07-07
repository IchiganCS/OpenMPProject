#include "Draw.h"

#include <SDL.h>
#include <SDL_pixels.h>
#include <SDL_surface.h>
#include <SDL_video.h>
#include <iostream>

using namespace std;

static SDL_Window* window;
static SDL_Renderer* renderer;

void initDrawing(int size)
{
    if (SDL_Init(SDL_INIT_VIDEO))
        cout << "SDL could not be initialized." << endl;

    window = SDL_CreateWindow("Flocking birds", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size, size, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

static void drawBirds(const vector<Bird>& birds, const SDL_Color& color = birdColor)
{
    if (birds.size() == 0)
        return;

    vector<SDL_Vertex> vertices;
    vertices.resize(birds.size() * 3);

    for (int birdCount = 0; birdCount < birds.size(); birdCount++)
    {
        const Bird& bird = birds[birdCount];
        for (int vertCount = 0; vertCount < 3; vertCount++)
        {
            // displace birdPosition
            // vertCount = 0 forward pointing
            // vertCount = 1, 2 point backward, 1 left, 2 right
            // such that a triangle is built
            Vec position = bird.position;
            switch (vertCount)
            {
            case 0:
                position.x += birdSize / 2 * cos(bird.angle);
                position.y += birdSize / 2 * sin(bird.angle);
                break;
            case 1: {
                float aimAngle = bird.angle + 3.141592 - birdSharpness / 2;
                position.x += birdSize / 2 * cos(aimAngle);
                position.y += birdSize / 2 * sin(aimAngle);
                break;
            }
            case 2: {
                float aimAngle = bird.angle + 3.141592 + birdSharpness / 2;
                position.x += birdSize / 2 * cos(aimAngle);
                position.y += birdSize / 2 * sin(aimAngle);
                break;
            }
            }
            vertices[birdCount * 3 + vertCount] = {{position.x, position.y}, color};
        }
    }

    SDL_RenderGeometry(renderer, nullptr, vertices.data(), vertices.size(), nullptr, 0);
}

static void drawPartitionLines(const std::vector<float>& lines)
{
    SDL_SetRenderDrawColor(renderer, partitionLineColor.r, partitionLineColor.g, partitionLineColor.b, partitionLineColor.a);
    for (auto& line : lines)
    {
        SDL_FRect goalBox;
        goalBox.x = line - partitionLineWidth / 2;
        goalBox.y = 0;

        int width, height;
        SDL_GetWindowSize(window, &width, &height);
        goalBox.h = height;
        goalBox.w = partitionLineWidth;
        SDL_RenderFillRectF(renderer, &goalBox);
    }
}
static void drawGoal(const Vec& position)
{
    SDL_SetRenderDrawColor(renderer, goalColor.r, goalColor.g, goalColor.b, goalColor.a);
    SDL_FRect goalBox;
    goalBox.x = position.x - goalSize / 2;
    goalBox.y = position.y - goalSize / 2;
    goalBox.h = goalSize;
    goalBox.w = goalSize;
    SDL_RenderFillRectF(renderer, &goalBox);
}

void drawOnlyBirds(const std::vector<Bird>& birds)
{

    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderClear(renderer);

    drawBirds(birds);

    SDL_RenderPresent(renderer);
}

void drawParallel(tuple<vector<Bird>, vector<float>, Vec, Bird> args)
{
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderClear(renderer);

    drawBirds(get<0>(args));
    drawGoal(get<2>(args));
    drawPartitionLines(get<1>(args));
    auto leader = get<3>(args);
    vector<Bird> leaderVec;
    leaderVec.push_back(leader);
    drawBirds(leaderVec, leaderColor);

    SDL_RenderPresent(renderer);
}