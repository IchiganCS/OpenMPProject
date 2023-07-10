#ifndef DRAW_H
#define DRAW_H

#include "Bird.h"
#include "ParAlgorithm.h"
#include <SDL_pixels.h>
#include <vector>

const float birdSize = 25.f;
const float goalSize = 20;
const float partitionLineWidth = 2;

// the angle between the two end vertices of the bird
// from the middle point
const float birdSharpness = 3.141 / 3;
const SDL_Color backgroundColor = {102, 204, 255, SDL_ALPHA_OPAQUE};
const SDL_Color birdColor = {139, 129, 126, SDL_ALPHA_OPAQUE};
const SDL_Color leaderColor = {0, 0, 0, SDL_ALPHA_OPAQUE};
const SDL_Color goalColor = {255, 0, 0, SDL_ALPHA_OPAQUE};
const SDL_Color partitionLineColor = {0, 0, 0, SDL_ALPHA_OPAQUE};
const SDL_Color obstacleColor = {255, 255, 255, SDL_ALPHA_OPAQUE};

void initDrawing(int size);

void drawOnlyBirds(const std::vector<Bird>& birds);
void drawParallel(const ParAlgorithm::DrawingInformation&);

#endif