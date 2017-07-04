#include "chip8.h"

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
        
        if(DEBUG)
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
