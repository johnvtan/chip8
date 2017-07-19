#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

void inputPollEvent(int keys[16]);
unsigned char inputWaitEvent(void);
#endif
