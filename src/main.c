#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 1000
#define HEIGHT 800
#define BUFFER_SIZE 256

char text_buffer[BUFFER_SIZE] = {0};
char text_row_buffer[BUFFER_SIZE] = {0};
int font_height;
int y = 0;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;
TTF_TextEngine *text_engine = NULL;

bool init() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL_INIT Error: %s\n", SDL_GetError());
    return false;
  }

  if (!TTF_Init()) {
    SDL_Log("TTF_INIT Error: %s\n", SDL_GetError());
    return false;
  }

  window = SDL_CreateWindow("Scratchpad", WIDTH, HEIGHT,
                            SDL_WINDOW_HIGH_PIXEL_DENSITY);
  if (window == NULL) {
    SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
    return false;
  }

  renderer = SDL_CreateRenderer(window, NULL);
  if (renderer == NULL) {
    SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    return false;
  }

  font = TTF_OpenFont("fonts/FiraCodeNerdFontMono-Regular.ttf", 36);
  if (font == NULL) {
    SDL_Log("TTF_OpenFont Error: %s\n", SDL_GetError());
    return false;
  }

  text_engine = TTF_CreateRendererTextEngine(renderer);
  if (text_engine == NULL) {
    SDL_Log("TTF_CreateSurfaceTextEngine Error: %s\n", SDL_GetError());
    return false;
  }

  font_height = TTF_GetFontHeight(font);
  return true;
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
  if (len == 0) {
    return '\0';
  }

  char c = text_buffer[len - 1];
  text_buffer[len - 1] = '\0';
  return c;
}

void render_text(const char *raw_text, SDL_Color text_color) {
  size_t len = strlen(raw_text);
  if (len == 0) {
    return;
  }

  int i = 0;
  int row = 0;
  int col = 0;
  TTF_Text *text = {};
  while (i < BUFFER_SIZE && i < len + 1) {
    if (text_buffer[i] == '\n' || text_buffer[i] == '\0') {
      text = TTF_CreateText(text_engine, font, text_row_buffer, strlen(text_row_buffer));
      TTF_DrawRendererText(text, 0, row);
      col = 0;
      ++row;
    }

    text_row_buffer[col] = text_buffer[i];
    text_row_buffer[col+1] = '\0';
    ++col;
    ++i;
  }

  TTF_DestroyText(text);
}

int main(int argc, char *argv[]) {

  if (!init()) {
    return 1;
  }

  bool done = false;
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
          if (y > 0) {
            y -= 1;
          }
          break;
        case SDLK_DOWN:
          if (y < HEIGHT / font_height - 1) {
            y += 1;
          }
          break;
        case SDLK_BACKSPACE:
          pop_char();
          break;
        case SDLK_1:
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
