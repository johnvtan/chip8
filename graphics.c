#include "graphics.h"

/* initializes window in SDL and returns pointer to it */
SDL_Window* graphicsInit(void)
{
   SDL_Window* window = NULL;

   if (SDL_Init(SDL_INIT_VIDEO) < 0)
   {
       printf("SDL couldn't initialize! SDL_Error: %s\n", SDL_GetError());
   }
   else
   {
       window = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_UNDEFINED, 
               SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH*SCALE, SCREEN_HEIGHT*SCALE,
               SDL_WINDOW_SHOWN);

       if (window == NULL)
       {
           printf("Window couldn't be created! SDL_Error: %s\n", SDL_GetError());
       }
       else
       {
       // Create the renderer
       SDL_Renderer *r = NULL;
       r = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
       
       // set color to black
       SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
       
       // clear the screen (needed?)
       SDL_RenderClear(r);

       // render the rect then render rect to screen
       SDL_RenderFillRect(r, NULL);
       SDL_RenderPresent(r);

       // destroy the renderer after we're done with it
       SDL_DestroyRenderer(r);
       }
   }

   return window;
}
  
/* updates the window to reflect the current graphics array */
void graphicsUpdate(unsigned char graphics[SCREEN_WIDTH][SCREEN_HEIGHT], SDL_Window *window)
{
    SDL_Renderer *r = NULL;
    r = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderClear(r);
    
    SDL_Rect currentPixel;
    int index;
    for (int i = 0; i < SCREEN_WIDTH; i++)
    {
        for (int j = 0; j < SCREEN_HEIGHT; j++)
        {
            // setting the position and dimensions of the current scaled pixel
            currentPixel.y = j * SCALE;
            currentPixel.x = i * SCALE;
            currentPixel.h = SCALE;
            currentPixel.w = SCALE;
            
            // Now need to call fillrect, depending on the value of the graphics
            // array - if 0, then black, if 1 then white
            if (graphics[i][j] == 1)
            {
                // renderer color set to white
                SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
            }
            else
            {
                // set renderer color to black
                SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
            }
            SDL_RenderFillRect(r, &currentPixel);
            SDL_RenderPresent(r);
        }
    }
    
    // deallocate the surface
    SDL_DestroyRenderer(r);
}
