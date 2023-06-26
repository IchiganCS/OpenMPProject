#ifndef MYRENDERER_H
#define MYRENDERER_H
#include "src/Bird.h"
#include <SDL2/SDL.h>
#include <string>
#include <vector>

class MyRenderer {
private:
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;

public:
  MyRenderer(std::string title);
  ~MyRenderer();
  void update(std::vector<Bird> &birds);
};
#endif