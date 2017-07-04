#include <string.h>
#include "chip8.h"
#ifdef DEBUG
#   include "test.h"
#endif

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Error: no filename included\n");
        exit(1);
    }

    chip8state *chip8;
    chip8init(chip8);
    char *filename = argv[1];
   
    // implementing test cases for debug mode
    if (DEBUG)
    {
        if (strcmp(filename, "test") == 0)
        {
            printf("Running test cases...\n");
            test(chip8);
            return 0;
        }
    }

    // if not test, then try programming and emulating w/ fetch-execute cycle
    program(filename, chip8);
    char c = 0;
    if (DEBUG)
    {
        printf("Debug mode! Press enter to step through the instructions.\n");
    }

    while (1)
    {
        fetch(chip8);
        execute(chip8);
        
        // Debug mode just makes the program step through the instructions
        // one at a time
        // TODO: write more robust debug mode, with step, run to pc == x
        // run until certain instruction, etc...
        if (DEBUG)
        {
            //printf("Press enter to continue, anything else to exit: ");
            printOpcode(chip8);
            c = getchar();
            if (c != '\n' && c != '\r')
            {
                printf("Exiting\n");
                break;
            }
        }
    }
        
    return 0;
}
