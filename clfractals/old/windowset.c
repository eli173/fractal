// Sets up window...
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>


void sdl_setup() {//DAMMIT screen needs to be global
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("Error:%s\n", SDL_GetError());
    return;
  }
  atexit(SDL_Quit);
  SDL_Surface *screen;
  screen = SDL_SetVideoMode(640, 480, 16, SDL_RESIZABLE);
  if (screen == NULL) {
    printf("Error:%s\n", SDL_GetError());
    return;
  }
}

void sdl_setpixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x;
  *p = pixel;
}

Uint32 rgb(SDL_Surface *surface, int r, int g, int b) {
  return SDL_MapRGB(screen
}
