#include "MyRenderer.h"
MyRenderer::MyRenderer(std::string &title) {
  window = SDL_CreateWindow(title.c_str(), 20, 20, 640, 480, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

MyRenderer::~MyRenderer() { SDL_DestroyWindow(window); }

void MyRenderer::update(std::vector<Bird> &birds) {}