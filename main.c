#include <string.h>
#include "chip8.h"
#include "test.h"
    
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Error: no filename included\n");
        exit(1);
    }

    chip8state chip8;
    chip8init(&chip8);
    char *filename = argv[1];
   
    // implementing test cases for debug mode
    if (DEBUG)
    {
        if (strcmp(filename, "test") == 0)
        {
            printf("Running test cases...\n");
            test(&chip8);
            return 0;
        }
    }

    // if not test, then try programming and emulating w/ fetch-execute cycle
    program(filename, &chip8);
    char c = 0;
    if (DEBUG)
    {
        printf("Debug mode! Press enter to step through the instructions.\n");
    }

    uint64_t delta_us = 0;
    struct timespec start_tp, end_tp; // used for keeping time
    clock_gettime(CLOCK_MONOTONIC_RAW, &start_tp);    
    //SDL_Event event;

    while (chip8.running)
    {
        // Delay needed so that the emulator doesn't run too fast
        SDL_Delay(1);

        // poll last key
        inputPollEvent(chip8.keys);
      
        /*
        for (int i = 0; i < 16; i++)
        {
            if (chip8.keys[i])
            {
                printf("Key %x pressed!\n", i);
            }
        }
        */
        fetch(&chip8);
        execute(&chip8);
        
        // calculating how much time has passed
        clock_gettime(CLOCK_MONOTONIC_RAW, &end_tp);

        // checking if current time > 16666 us (ie, ~1/60 of 1 sec)
        delta_us = (end_tp.tv_sec - start_tp.tv_sec) * 1000000 + (end_tp.tv_nsec - start_tp.tv_nsec) / 1000;

        if ((delta_us) > 16666)
        {
            if (chip8.delay > 0)
            {
                chip8.delay--;
            }
            if (chip8.sound > 0)
            {
                chip8.sound--;
            }
           
            start_tp = end_tp;
        }
        
        // Debug mode just makes the program step through the instructions
        // one at a time - note that this will definitely mess up the timer, 
        // since that checks the actual time that passes
        
        if (DEBUG)
        {
            //printf("Press enter to continue, anything else to exit: ");
            printOpcode(&chip8);
            c = getchar();
            if (c != '\n' && c != '\r')
            {
                printf("Exiting\n");
                break;
            }
        }
        
    }
        
    SDL_DestroyRenderer(chip8.renderer);
    return 0;
}
