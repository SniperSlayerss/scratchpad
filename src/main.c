#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 1000
#define HEIGHT 800

int init(SDL_Window **window, SDL_Renderer **renderer) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL_INIT: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  if (!TTF_Init()) {
    SDL_Log("TTF_INIT: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  *window = SDL_CreateWindow("Scratchpad", WIDTH, HEIGHT, 0);
  if (window == NULL) {
    SDL_Log("SDL_CreateWindow: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  *renderer = SDL_CreateRenderer(*window, NULL);
  if (renderer == NULL) {
    SDL_Log("SDL_CreateRenderer: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
  SDL_Window *window;
  SDL_Renderer *renderer;
  bool done = false;

  if (init(&window, &renderer) == EXIT_FAILURE) {
    return EXIT_FAILURE;
  }

  // Main loop
  while (!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        done = true;
      }
    }

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
