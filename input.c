#include "input.h"

/* translates key press into appropriate char (0-F). If key not pressed, or
 * unknown key pressed, then return -1*/
void inputPollEvent(int keys[16])
{
    // setting all keys to be 0
    //memset(keys, 0, 16 * sizeof(int));
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        //printf("Event found...\n");
        switch (event.type)
        {
            case SDL_QUIT:
                {
                    printf("Exiting Chip8...\n");
                    exit(1);
                }
            case SDL_KEYUP:
            case SDL_KEYDOWN:
                {
                    //printf("Keydown event!\n");
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_1:
                            {
                                keys[1] = (event.type == SDL_KEYDOWN) ? 1 : 0;
                                break;
                            }
                        case SDLK_2: 
                            {
                                keys[2] = (event.type == SDL_KEYDOWN) ? 1 : 0;
                                break;
                            }
                        case SDLK_3: 
                            {
                                keys[3] = (event.type == SDL_KEYDOWN) ? 1 : 0;
                                break;
                            }
                        case SDLK_4:
                            {
                                keys[0xC] = (event.type == SDL_KEYDOWN) ? 1 : 0;
                                break;
                            }
                        case SDLK_q:
                            {
                                keys[4] = (event.type == SDL_KEYDOWN) ? 1 : 0;
                                break;
                            }
                        case SDLK_w: 
                            {
                                keys[5] = (event.type == SDL_KEYDOWN) ? 1 : 0;
                                break;
                            }
                        case SDLK_e:
                            {
                                keys[6] = (event.type == SDL_KEYDOWN) ? 1 : 0;
                                break;
                            }
                        case SDLK_r:
                            {
                                keys[0xD] = (event.type == SDL_KEYDOWN) ? 1 : 0;
                                break;
                            }
                        case SDLK_a: 
                            {
                                keys[7] = (event.type == SDL_KEYDOWN) ? 1 : 0;
                                break;
                            }
                        case SDLK_s: 
                            {
                                keys[8] = (event.type == SDL_KEYDOWN) ? 1 : 0;
                                break;
                            }
                        case SDLK_d: 
                            {
                                keys[9] = (event.type == SDL_KEYDOWN) ? 1 : 0;
                                break;
                            }
                        case SDLK_f: 
                            {
                                keys[0xE] = (event.type == SDL_KEYDOWN) ? 1 : 0;
                                break;
                            }
                        case SDLK_z: 
                             {
                                 keys[0xA] = (event.type == SDL_KEYDOWN) ? 1 : 0;
                                 break;
                             }            
                        case SDLK_x: 
                             {
                                 keys[0] = (event.type == SDL_KEYDOWN) ? 1 : 0;
                                 break;
                             }
                        case SDLK_c:
                             {
                                 keys[0xB] = (event.type == SDL_KEYDOWN) ? 1 : 0;
                                 break;
                             }
                        case SDLK_v: 
                             {
                                 keys[0xF] = (event.type == SDL_KEYDOWN) ? 1 : 0;

                                 break;
                             }
                        default:
                             // do nothing if unknown key press
                             break;
                            
                    }
                }
            default:
                break;
        }
    }
}

/* stops execution until some (valid) button is pressed - returns that value */
unsigned char inputWaitEvent(void)
{
    SDL_Event event;
    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_1: return 1;
                    case SDLK_2: return 2;
                    case SDLK_3: return 3;
                    case SDLK_4: return 0xC;
                    case SDLK_q: return 4;
                    case SDLK_w: return 5;
                    case SDLK_e: return 6;
                    case SDLK_r: return 0xD;
                    case SDLK_a: return 7;
                    case SDLK_s: return 8;
                    case SDLK_d: return 9;
                    case SDLK_f: return 0xE;
                    case SDLK_z: return 0xA;
                    case SDLK_x: return 0;
                    case SDLK_c: return 0xB;
                    case SDLK_v: return 0xF;
                }
            }
        }
    }

    // should never get here! 
    return 0;
}
