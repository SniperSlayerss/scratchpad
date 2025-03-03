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
#define BUFFER_SIZE 256

const int pixel_size = 37;
int y = 0;
char text_buffer[BUFFER_SIZE] = {0};

SDL_Window *window     = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font         = NULL;

int init() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL_INIT Error: %s\n", SDL_GetError());
    return 0;
  }

  if (!TTF_Init()) {
    SDL_Log("TTF_INIT Error: %s\n", SDL_GetError());
    return 0;
  }

  window = SDL_CreateWindow("Scratchpad", WIDTH, HEIGHT, 0);
  if (window == NULL) {
    SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
    return 0;
  }

  renderer = SDL_CreateRenderer(window, NULL);
  if (renderer == NULL) {
    SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    return 0;
  }

  font = TTF_OpenFont("fonts/FiraCodeNerdFontMono-Regular.ttf", 28);
  if (font == NULL) {
    SDL_Log("TTF_OpenFont Error: %s\n", SDL_GetError());
    return 0;
  }

  return 1;
}

void append_char(char c) {
  size_t len = strlen(text_buffer);
  if (len >= BUFFER_SIZE - 1) {
    return;
  }

  text_buffer[len] = c;
  text_buffer[len + 1] = '\0';
}

char pop_char() {
  size_t len = strlen(text_buffer);
  if (len == 0 ) {
    return '\0';
  }

  char c = text_buffer[len - 1];
  text_buffer[len - 1] = '\0';
  return c;
}

void render_text(const char *text, SDL_Color text_color) {
  size_t len = strlen(text_buffer);
  if (len == 0) {
    return;
  }

  // New line sort out test
  SDL_Surface *surface = TTF_RenderText_Blended(font, text, len, text_color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FRect dstrect = {0, pixel_size * y, (float) surface->w, pixel_size};
  SDL_RenderTexture(renderer, texture, NULL, &dstrect);

  // draw to screen
  SDL_DestroySurface(surface);
  SDL_DestroyTexture(texture);
}

int main(int argc, char *argv[]) {

  if (!init()) {
    return 1;
  }

  _Bool done = false;
  while (!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        done = true;
      }

      if (event.type == SDL_EVENT_TEXT_INPUT) {
        append_char(event.text.text[0]);
      }

      if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
          case SDLK_UP:
            if (y > 0){
              y -= 1;
            }
            break;
          case SDLK_DOWN:
            if (y < HEIGHT / pixel_size - 1) {
              y += 1;
            }
            break;
          case SDLK_BACKSPACE:
            pop_char();
            break;
          case SDLK_KP_ENTER:
            append_char('\n');
          case SDLK_I:
            SDL_StartTextInput(window);
            break;
          case SDLK_ESCAPE:
            SDL_StopTextInput(window);
            break;
        }
      }
    }

    // clear window to grey
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);

    SDL_Color text_color = {224, 222, 244};
    render_text(text_buffer, text_color);


    SDL_RenderPresent(renderer);
  }

  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
