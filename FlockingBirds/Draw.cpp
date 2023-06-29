#include "Draw.h"

#include <SDL.h>
#include <iostream>

using namespace std;

const float birdSize = 25.f;
// the angle between the two end vertices of the bird
// from the middle point
const float birdSharpness = 3.141 / 3;
const SDL_Color backgroundColor = {102, 204, 255, SDL_ALPHA_OPAQUE};
const SDL_Color birdColor = {255, 255, 255, SDL_ALPHA_OPAQUE};

static SDL_Window* window;
static SDL_Renderer* renderer;

void init()
{
    if (SDL_Init(SDL_INIT_VIDEO))
        cout << "SDL could not be initialized." << endl;

    window = SDL_CreateWindow("Flocking birds", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void draw(const vector<Bird>& birds)
{
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderClear(renderer);

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
                position.X += birdSize / 2 * cos(bird.angle);
                position.Y += birdSize / 2 * sin(bird.angle);
                break;
            case 1: {
                float aimAngle = bird.angle + 3.141592 - birdSharpness / 2;
                position.X += birdSize / 2 * cos(aimAngle);
                position.Y += birdSize / 2 * sin(aimAngle);
                break;
            }
            case 2: {
                float aimAngle = bird.angle + 3.141592 + birdSharpness / 2;
                position.X += birdSize / 2 * cos(aimAngle);
                position.Y += birdSize / 2 * sin(aimAngle);
                break;
            }
            }
            vertices[birdCount * 3 + vertCount] = {{position.X, position.Y}, birdColor};
        }
    }

    SDL_RenderGeometry(renderer, nullptr, vertices.data(), vertices.size(), nullptr, 0);

    SDL_RenderPresent(renderer);
}