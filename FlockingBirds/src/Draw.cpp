
#include "Draw.h"
#ifndef EXCLUDESDL
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

  window = SDL_CreateWindow("Flocking birds - RED - Alignment factor, Green - "
                            "Sepration factor, Blue - Cohesion factor",
                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            Utils::WINDOW_WIDTH, Utils::WINDOW_HEIGHT, 0);
  Utils::DETAILS_WIDTH = Utils::WINDOW_WIDTH - Utils::WINDOW_HEIGHT;
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

static int SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y,
                                int radius) {
  int offsetx, offsety, d;
  int status;

  // CHECK_RENDERER_MAGIC(renderer, -1);

  offsetx = 0;
  offsety = radius;
  d = radius - 1;
  status = 0;

  while (offsety >= offsetx) {

    status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                 x + offsety, y + offsetx);
    status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                 x + offsetx, y + offsety);
    status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                 x + offsetx, y - offsety);
    status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                 x + offsety, y - offsetx);

    if (status < 0) {
      status = -1;
      break;
    }

    if (d >= 2 * offsetx) {
      d -= 2 * offsetx + 1;
      offsetx += 1;
    } else if (d < 2 * (radius - offsety)) {
      d += 2 * offsety - 1;
      offsety -= 1;
    } else {
      d += 2 * (offsety - offsetx - 1);
      offsety -= 1;
      offsetx += 1;
    }
  }

  return status;
}

static void drawObstacle(const Obstacle &obstacle) {
  SDL_SetRenderDrawColor(renderer, obstacleColor.r, obstacleColor.g,
                         obstacleColor.b, obstacleColor.a);
  SDL_RenderFillCircle(renderer, obstacle.center.x, obstacle.center.y,
                       obstacle.radius);
  return;
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

void drawOnlyBirds(const std::vector<Bird> &birds,
                   std::vector<Obstacle> &obstacles) {

  SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g,
                         backgroundColor.b, backgroundColor.a);
  SDL_RenderClear(renderer);
  drawBirds(birds);
  // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  for (Obstacle &obstacle : obstacles)
    drawObstacle(obstacle);

  SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
  SDL_FRect goalBox;
  goalBox.x = Utils::WINDOW_HEIGHT;
  goalBox.y = 0;
  goalBox.h = Utils::WINDOW_HEIGHT;
  goalBox.w = Utils::DETAILS_WIDTH;
  SDL_RenderFillRectF(renderer, &goalBox);

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_FRect align_factor;
  align_factor.x = Utils::WINDOW_HEIGHT;
  align_factor.y = 0;
  align_factor.h =
      (Utils::WINDOW_HEIGHT * float(float(Utils::ALIGNMENT_FACTOR) / 100.00f));
  align_factor.w = Utils::DETAILS_WIDTH * 0.25;
  SDL_RenderFillRectF(renderer, &align_factor);

  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_FRect sepration;
  sepration.x = Utils::WINDOW_HEIGHT + Utils::DETAILS_WIDTH * 0.25;
  sepration.y = 0;
  sepration.h =
      (Utils::WINDOW_HEIGHT * float(float(Utils::SEPRATION_FACTOR) / 100.00f));
  sepration.w = Utils::DETAILS_WIDTH * 0.25;
  SDL_RenderFillRectF(renderer, &sepration);

  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_FRect cohesion_factor;
  cohesion_factor.x = Utils::WINDOW_HEIGHT + Utils::DETAILS_WIDTH * 0.50;
  cohesion_factor.y = 0;
  cohesion_factor.h =
      (Utils::WINDOW_HEIGHT * float(float(Utils::COHESION_FACTOR) / 100.00f));
  cohesion_factor.w = Utils::DETAILS_WIDTH * 0.25;
  SDL_RenderFillRectF(renderer, &cohesion_factor);

  SDL_SetRenderDrawColor(renderer, 0, 111, 255, 255);
  SDL_FRect avoidance_factor;
  avoidance_factor.x = Utils::WINDOW_HEIGHT + Utils::DETAILS_WIDTH * 0.75;
  avoidance_factor.y = 0;
  avoidance_factor.h =
      (Utils::WINDOW_HEIGHT * float(float(Utils::AVOIDANCE_FACTOR) / 100.00f));
  avoidance_factor.w = Utils::DETAILS_WIDTH * 0.25;
  SDL_RenderFillRectF(renderer, &avoidance_factor);

  SDL_RenderPresent(renderer);
}

#endif