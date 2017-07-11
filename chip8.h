#ifndef CHIP8_H
#define CHIP8_H
/* includes types, instantiations of all the chip8 parts */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memset
#include <time.h>
#include "graphics.h"
#include "input.h"

/* MACRO DEFINITIONS */
// DEBUG stuff - used -DDEBUG flag when compiling to turn this on
#ifdef DEBUG
// the do-while loop ensures that the code acts like a function call?
// so a call would be like DEBUG_PRINT("%s, %s", s1, s2)
#define DEBUG_PRINT(fmt, ...) do{ fprintf(stderr, fmt, __VA_ARGS__); } while(0)
#else
#define DEBUG_PRINT(...)  // do nothing if not in debug mode
#endif

// GET_X, GET_Y cast as ints since they are used as indexes
#define GET_X(opcode) (int) ((opcode & 0x0F00) >> 8)
#define GET_Y(opcode) (int) ((opcode & 0x00F0) >> 4)

// GET_KK, GET_NNN cast as char, short to fit the data size
#define GET_KK(opcode) (unsigned char) (opcode & 0x00FF)
#define GET_NNN(opcode) (unsigned short) (opcode & 0x0FFF)

/* important to note that short is 2 bytes, long is 4 bytes */

typedef struct 
{
    // flag to tell if the game is running or we need to quit
    int running;
    unsigned char memory[4096];

    // general purpose registers
    unsigned char V[16];

    // I used for storing memory address - only lower 3 bytes used
    unsigned short I;
    unsigned short pc;
    unsigned char graphics[64][32];
    unsigned char delay;
    unsigned char sound;
    unsigned short stack[16];
    unsigned char sp;

    // keeps track of current opcode
    unsigned short opcode;

    // Also has pointer to its own renderer
    SDL_Renderer* renderer;

    // keeping track of last pressed button
    int keys[16];
} chip8state;

/* all the fonts (for 0-F) that need to be loaded into chip8 memory */
static const unsigned char chip8fonts[80] = { 0xF0, 0x90, 0x90, 0x90, 0xF0,
                                              0x20, 0x60, 0x20, 0x20, 0x70,
                                              0xF0, 0x10, 0xF0, 0x80, 0xF0,
                                              0xF0, 0x10, 0xF0, 0x10, 0xF0,
                                              0x90, 0x90, 0xF0, 0x10, 0x10,
                                              0xF0, 0x80, 0xF0, 0x10, 0xF0,
                                              0xF0, 0x80, 0xF0, 0x90, 0xF0,
                                              0xF0, 0x10, 0x20, 0x40, 0x40,
                                              0xF0, 0x90, 0xF0, 0x90, 0xF0,
                                              0xF0, 0x90, 0xF0, 0x10, 0xF0,
                                              0xF0, 0x90, 0xF0, 0x90, 0x90,
                                              0xE0, 0x90, 0xE0, 0x90, 0xE0,
                                              0xF0, 0x80, 0x80, 0x80, 0xF0,
                                              0xE0, 0x90, 0x90, 0x90, 0xE0,
                                              0xF0, 0x80, 0xF0, 0x80, 0xF0,
                                              0xF0, 0x80, 0xF0, 0x80, 0x80 };

/* main functions */
// loads program in file into memory
void chip8init(chip8state *chip8);
void program(char* filename, chip8state* chip8);
void fetch(chip8state* chip8);
void execute(chip8state* chip8);

/* helper functions */
unsigned char readMem(int address, chip8state* chip8);
void writeMem(int address, unsigned char val, chip8state* chip8);
unsigned char readReg(int x, chip8state* chip8);
void writeReg(int x, unsigned char val, chip8state* chip8);
void unhandledInstruction(char *msg, chip8state* chip8);
void printMemory(int address, int period, chip8state* chip8);
void printOpcode(chip8state* chip8);
void printError(char *msg, chip8state* chip8);

#endif
