#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>

/* macro definitions for screen size */
#define SCALE 10
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

/* graphics functions only need to look at the graphics array, not whole chip8
 * struct */
SDL_Window* graphicsInit(void);
void graphicsUpdate(unsigned char graphics[SCREEN_WIDTH][SCREEN_HEIGHT], SDL_Window *window);

#endif
