#include "chip8.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Error: no filename included\n");
        exit(1);
    }

    chip8init();
    char *filename = argv[1];
    program(filename);
    printMemory(0x200, 63);

    writeMem(0x200, 0xff);
    printf("%x\n", readMem(0x200));

    writeMem(0x200, 0x01);
    printf("%x\n", readMem(0x200));

    writeReg(0, 0xff);
    printf("%x\n", readReg(0));

    writeReg(0, 0x2b);
    printf("%x\n", readReg(0));

    return 0;
}
