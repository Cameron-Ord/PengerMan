#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#define PENGER_FRAME_TIME 32

void error_callback(const char *msg);

int main(int argc, char **argv) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
    error_callback(SDL_GetError());
    exit(EXIT_FAILURE);
  }

  if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)) {
    error_callback(SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  SDL_Window *w =
      SDL_CreateWindow("Penger mans", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_HIDDEN);

  SDL_Renderer *r = SDL_CreateRenderer(
      w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  SDL_Surface *penger_surface = IMG_Load("gfx/SuperPenger.png");
  SDL_Surface *bg_surface = IMG_Load("gfx/bg.jpg");
  if (!bg_surface) {
    error_callback(SDL_GetError());
  }

  SDL_Texture *penger_texture = SDL_CreateTextureFromSurface(r, penger_surface);
  SDL_Texture *bg_texture = SDL_CreateTextureFromSurface(r, bg_surface);

  if (!bg_texture) {
    error_callback(SDL_GetError());
  }

  int offset_x = 600 / 2;
  int offset_y = 400 / 2;

  int tile_size = 128;
  int sprite_size = 64;

  int x = 1;
  int y = 1;

  int boolet_y;
  int boolet_x;

  SDL_ShowWindow(w);

  int running = 1;
  int FPS = 60;

  int is_boolet = 0;

  const int ticks_per_frame = (1000.0 / FPS);
  uint64_t frame_start;
  int frame_time;

  int penger_boolet_frame = 0;

  while (running) {

    frame_start = SDL_GetTicks64();
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      default:
        break;
      case SDL_KEYDOWN: {
        switch (e.key.keysym.sym) {
        default:
          break;

        case SDLK_f: {
          if (!is_boolet) {
            boolet_x = x;
            boolet_y = y;

            printf("%d - %d\n", boolet_x, boolet_y);
            is_boolet = 1;
          }
        } break;

        case SDLK_UP: {
          y -= 1;
        } break;
        case SDLK_DOWN: {
          y += 1;
        } break;

        case SDLK_LEFT: {
          x -= 1;
        } break;

        case SDLK_RIGHT: {
          x += 1;
        } break;
        }
      } break;
      case SDL_QUIT: {
        running = 0;
      } break;
      }
    }

    SDL_RenderClear(r);

    SDL_Rect BGRECT = {0, 0, 600, 400};
    int err = SDL_RenderCopy(r, bg_texture, NULL, &BGRECT);

    int tile_y = (x + y) * (tile_size / 2);
    int tile_x = (x - y) * (tile_size / 2);

    int penger_x = tile_x + (tile_size - sprite_size) / 2;
    int penger_y = tile_y + (tile_size - sprite_size) / 2;

    SDL_Rect penger_rect = {penger_x, penger_y, sprite_size, sprite_size};

    SDL_RenderCopy(r, penger_texture, NULL, &penger_rect);

    if (is_boolet) {
      boolet_x += 1;
      boolet_y -= 1;

      tile_y = (boolet_x + boolet_y) * (tile_size / 2);
      tile_x = (boolet_x - boolet_y) * (tile_size / 2);

      int penger_boolet_x = tile_x + (tile_size - sprite_size) / 2;
      int penger_boolet_y = tile_y + (tile_size - sprite_size) / 2;

      SDL_Rect penger_boolet_rect = {penger_boolet_x, penger_boolet_y, 64, 64};

      SDL_RenderCopy(r, penger_texture, NULL, &penger_boolet_rect);
      penger_boolet_frame++;
    }

    if (penger_boolet_frame > PENGER_FRAME_TIME) {
      is_boolet = 0;
      penger_boolet_frame = 0;
    }

    frame_time = SDL_GetTicks64() - frame_start;
    if (ticks_per_frame > frame_time) {
      SDL_Delay(ticks_per_frame - frame_time);
    }

    SDL_RenderPresent(r);
  }
  return 0;
}

void error_callback(const char *msg) { fprintf(stderr, "%s\n", msg); }
