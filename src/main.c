#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#define WIDTH 1000
#define HEIGHT 800
#define BUFFER_SIZE 1024

typedef struct {
  size_t cursor_pos;
  size_t len;
  char text_buffer[BUFFER_SIZE];
} FileContext;

char text_row_buffer[BUFFER_SIZE] = {0};

int font_height;

SDL_Window     *window      = NULL;
SDL_Renderer   *renderer    = NULL;
TTF_Font       *font        = NULL;
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

  font = TTF_OpenFont("./fonts/FiraCodeNerdFontMono-Regular.ttf", 36);
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

void cleanup() {
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

bool context_append_char(FileContext *context, char c) {
  if (context->len >= BUFFER_SIZE - 1) {
    return false;
  }

  context->text_buffer[context->len] = c;
  context->text_buffer[context->len + 1] = '\0';
  ++context->len;
  return true;
}

char context_pop_char(FileContext *context) {
  if (context->len == 0) {
    return '\0';
  }

  char c = context->text_buffer[context->len - 1];
  context->text_buffer[context->len - 1] = '\0';
  --context->len;
  return c;
}

bool context_render_text(FileContext *context) {
  if (context->len == 0) {
    return false;
  }

  int i = 0;
  int row = 0;
  int col = 0;
  TTF_Text *text = {};
  while (i < BUFFER_SIZE && i < context->len + 1) {
    if (context->text_buffer[i] == '\n' || context->text_buffer[i] == '\0') {
      text = TTF_CreateText(text_engine, font, text_row_buffer, col);
      TTF_DrawRendererText(text, 0, row * font_height);
      col = 0;
      ++row;
      ++i;
    }

    text_row_buffer[col] = context->text_buffer[i];
    text_row_buffer[col+1] = '\0';
    ++col;
    ++i;
  }

  TTF_DestroyText(text);
  return true;
}

int main(int argc, char *argv[]) {
  if (!init()) {
    return 1;
  }

  FileContext context = {0, 0, {0}};

  bool done = false;
  while (!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        done = true;
      }
      // ~~INSERT MODE~~
      if (event.type == SDL_EVENT_TEXT_INPUT) {
        context_append_char(&context, event.text.text[0]);
      }
      // ~~NORMAL MODE~~
      if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
        case SDLK_BACKSPACE:
          context_pop_char(&context);
          break;
        case SDLK_RETURN:
          context_append_char(&context, '\n');
        case SDLK_I:
          SDL_StartTextInput(window);
          break;
        case SDLK_ESCAPE:
          SDL_StopTextInput(window);
          break;
        }
      }
    }
    // Background
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderClear(renderer);

    // Text
    // TODO: Change text color
    SDL_Color text_color = {224, 222, 244, 255};
    context_render_text(&context);

    // Render
    SDL_RenderPresent(renderer);
  }

  cleanup();
  return 0;
}
