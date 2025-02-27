#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 1000
#define HEIGHT 800

int init(SDL_Window **window, SDL_Renderer **renderer) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL_INIT Error: %s\n", SDL_GetError());
    return 0;
  }

  if (!TTF_Init()) {
    SDL_Log("TTF_INIT Error: %s\n", SDL_GetError());
    return 0;
  }

  *window = SDL_CreateWindow("Scratchpad", WIDTH, HEIGHT, 0);
  if (window == NULL) {
    SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
    return 0;
  }

  *renderer = SDL_CreateRenderer(*window, NULL);
  if (renderer == NULL) {
    SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    return 0;
  }
  return 1;
}

int main(int argc, char *argv[]) {
  SDL_Window *window;
  SDL_Renderer *renderer;
  _Bool done = false;

  if (!init(&window, &renderer)) {
    return 1;
  }

  TTF_Font *font = TTF_OpenFont("fonts/FiraCodeNerdFontMono-Regular.ttf", 24);
  if (font == NULL) {
    SDL_Log("TTF_OpenFont Error: %s\n", SDL_GetError());
    return 0;
  }

  // Main loop
  while (!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        done = true;
      }
    }

    // clear window to grey
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);

    SDL_Color text_color = {224, 222, 244};
    const char *text = "The quick brown fox jumped over the lazy fox...";

    SDL_Surface *surface =
        TTF_RenderText_Blended(font, text, strlen(text), text_color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FRect dstrect = {0, 0, (float) surface->w, (float) surface->h};
    SDL_RenderTexture(renderer, texture, NULL, &dstrect);

    // draw to screen
    SDL_RenderPresent(renderer);
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
  }

  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
