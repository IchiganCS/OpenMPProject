#include "MyRenderer.h"
#include "src/utils/Utils.h"

int SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y, int radius) {
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

MyRenderer::MyRenderer(std::string title) {
  SDL_DisplayMode dm;

  if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
    SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
  }

  int w, h;
  w = dm.w;
  h = dm.h;
  Utils::WINDOW_HEIGHT = 0.8 * dm.h;
  Utils::WINDOW_WIDTH = 0.8 * dm.w;
  window = SDL_CreateWindow(title.c_str(), 20, 20, Utils::WINDOW_WIDTH,
                            Utils::WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

MyRenderer::~MyRenderer() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void MyRenderer::update(std::vector<Bird> &birds) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  for (Bird b : birds) {
    SDL_RenderFillCircle(renderer, b.position.X, b.position.Y, 3);
  }

  SDL_RenderPresent(renderer);

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      exit(0);
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        exit(0);
      }
      break;
    }
  }
}