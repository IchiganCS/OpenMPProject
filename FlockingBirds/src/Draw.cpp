#include "Draw.h"
#include "Obstacle.h"
#include "utils/Utils.h"

#include <SDL.h>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_video.h>
#include <cmath>
#include <iostream>
#include <type_traits>
#include <vector>

using namespace std;

static SDL_Window *window;
static SDL_Renderer *renderer;

void initDrawing(int size) {
  if (SDL_Init(SDL_INIT_VIDEO))
    cout << "SDL could not be initialized." << endl;

  window = SDL_CreateWindow("Flocking birds", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, Utils::WINDOW_HEIGHT,
                            Utils::WINDOW_HEIGHT, 0);
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

// Builds a triangle for each bird and renders it.
static void drawBirds(const vector<Bird> &birds,
                      const SDL_Color &color = birdColor,
                      bool drawWithAngle = true) {
  if (birds.size() == 0)
    return;

  vector<SDL_Vertex> vertices;
  vertices.resize(birds.size() * 3);

  for (int birdCount = 0; birdCount < birds.size(); birdCount++) {
    const Bird &bird = birds[birdCount];
    for (int vertCount = 0; vertCount < 3; vertCount++) {
      // displace birdPosition
      // if the bird should be angled:
      // vertCount = 0 forward pointing
      // vertCount = 1, 2 point backward, 1 left, 2 right
      Vec position = bird.position;
      if (!drawWithAngle)
        switch (vertCount) {
        case 0:
          position.y += birdSize / 2;
          break;
        case 1: {
          float aimAngle = -3.141592 / 180 * 30;
          position.x += birdSize / 2 * cos(aimAngle);
          position.y += birdSize / 2 * sin(aimAngle);
          break;
        }
        case 2: {
          float aimAngle = -3.141592 / 180 * 150;
          position.x += birdSize / 2 * cos(aimAngle);
          position.y += birdSize / 2 * sin(aimAngle);
          break;
        }
        }
      else
        switch (vertCount) {
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

  SDL_RenderGeometry(renderer, nullptr, vertices.data(), vertices.size(),
                     nullptr, 0);
}

static void drawObstacle(const Obstacle &obstacle) {
  SDL_SetRenderDrawColor(renderer, obstacleColor.r, obstacleColor.g,
                         obstacleColor.b, obstacleColor.a);

  const int resolution = 360;
  vector<SDL_Vertex> vertices;
  vertices.reserve(resolution + 1);
  vertices.push_back({{obstacle.center.x, obstacle.center.y}, obstacleColor});
  float currentAngle = 0;
  float increment = 2 * (22 / 7) / (resolution + 1);

  for (int i = 0; i < resolution; i++) {
    SDL_Vertex point;
    point.position.x = obstacle.center.x + cos(currentAngle) * obstacle.radius;
    point.position.y = obstacle.center.y + sin(currentAngle) * obstacle.radius;
    point.color = obstacleColor;
    vertices.push_back(point);
    currentAngle += increment;
  }

  vector<int> indices;
  indices.reserve(resolution * 3);

  for (int i = 0; i < resolution; i++) {
    indices.push_back(0);
    indices.push_back(i + 1);
    if (i + 2 >= resolution + 1)
      indices.push_back(1);
    else
      indices.push_back(i + 2);
  }

  SDL_RenderGeometry(renderer, nullptr, vertices.data(), vertices.size(),
                     indices.data(), indices.size());
}

static void drawPartitionLines(const std::vector<float> &lines) {
  SDL_SetRenderDrawColor(renderer, partitionLineColor.r, partitionLineColor.g,
                         partitionLineColor.b, partitionLineColor.a);
  for (auto &line : lines) {
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
static void drawGoal(const Vec &position) {
  SDL_SetRenderDrawColor(renderer, goalColor.r, goalColor.g, goalColor.b,
                         goalColor.a);
  SDL_FRect goalBox;
  goalBox.x = position.x - goalSize / 2;
  goalBox.y = position.y - goalSize / 2;
  goalBox.h = goalSize;
  goalBox.w = goalSize;
  SDL_RenderFillRectF(renderer, &goalBox);
}

void drawOnlyBirds(const std::vector<Bird> &birds) {

  SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g,
                         backgroundColor.b, backgroundColor.a);
  SDL_RenderClear(renderer);

  drawBirds(birds);

  SDL_RenderPresent(renderer);
}