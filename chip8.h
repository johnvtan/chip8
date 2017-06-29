/* includes types, instantiations of all the chip8 parts */
#include <stdio.h>
#include <stdlib.h>
//#include "instructions.h"

/* important to note that short is 2 bytes, long is 4 bytes */

typedef struct 
{
    unsigned char memory[4096];

    // general purpose registers
    unsigned char V[16];

    // I used for storing memory address - only lower 3 bytes used
    unsigned short I;
    unsigned short pc;
    unsigned char graphics[64 * 32];
    unsigned char delay;
    unsigned char sound;
    unsigned short stack;
    unsigned char sp;

    // keeps track of current opcode
    unsigned short opcode;
} chip8state;

/* main functions */
// loads program in file into memory
void chip8init(void);
void program(char* filename);
void fetch(void);
void execute(void);

/* helper functions */
unsigned char readMem(int address);
void writeMem(int address, unsigned char val);
unsigned char readReg(int x);
void writeReg(int x, unsigned char val);
void printMemory(int address, int period);
void printOpcode(void);
void printError(char *msg);
