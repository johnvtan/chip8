#include "chip8.h"

/* global chip8state, local to this file */
static chip8state chip8;

/* initializes chip8state to correct initial values */
void chip8init(void)
{
    //TODO: finish initialization
    chip8.pc = 0x200;
}

/* reads file into memory byte by byte */
void program(char* filename)
{
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        perror("Opening input file failed!");
        exit(1);
    }

    int c;
    int pc = 0x200;     // local program counter, starting at 200
    while ((c = fgetc(fp)) != EOF)
    {
        if (pc > 4096)
        {
            printf("Error: program (%s) too large to fit in memory.\n", filename);
            exit(1);
        }
        
        // write the char into memory
        chip8.memory[pc] = (unsigned char) c;
        pc++;
    }
}

/* reads next two chars from memory, then increments pc by 2 */
void fetch(void)
{
    // left shifting higher address (MSB = leftmost bit) 
    chip8.opcode = (chip8.memory[chip8.pc]) | (chip8.memory[chip8.pc + 1] << 8);
    chip8.pc += 2;
}

/* executes the current instruction */
void execute(void)
{
    return;
}

/*********** Helper functions *********************************/
/* function to read from memory */
unsigned char readMem(int address)
{
    if (address >= 0 && address <= 4096)
    {
        return chip8.memory[address];
    }
    else
    {
        printError("Memory access error.");
        return -1;
    }
}

/* writes to specified memory address */
void writeMem(int address, unsigned char val)
{
    if (address >= 0 && address <= 4096)
    {
        chip8.memory[address] = val;
    }
    else
    {
        printError("Memory access error.");
    }
}

/* reads from register Vx */
unsigned char readReg(int x)
{
    if (x >= 0 && x <= 15)
    {
        return chip8.V[x];
    }
    else
    {
        printError("Register access errror.");
        return -1;
    }
}

/* writes value to register Vx */
void writeReg(int x, unsigned char val)
{
    if (x >= 0 && x <= 15)
    {
        chip8.V[x] = val;
    }
    else
    {
        printError("Register access error.");
    }
}

/* helper function, for debugging/testing */
void printMemory(int address, int period)
{
    int count = 0;
    char c = 0;
    
    while (1)
    {
        printf("\nMemory locations from 0x%x to 0x%x\n", address, address + period);

        // printing out memory locations, 16 per line
        for (count = 0; count <= period; count++)
        {
            printf("%02x", chip8.memory[address + count]);
            
            // formatting - tabs every 8th, endline every 16th
            if ( (count+1) % 2 == 0) 
            {
                printf(" ");
            }
            if ( (count+1) % 16 == 0)
            {
                printf("\n");
            }
        }

        // breakout condition
        
        printf("\nPress enter to continue, any other key to exit:");
        c = getchar();
        if (c == '\r' || c == '\n') 
        {
            address += period + 1; 
        }
        else
        {
            break;
        }
  
    }
}

void printOpcode(void)
{
    printf("%04x\n", chip8.opcode);
}

/* printing error msg w/ current pc, opcode */
void printError(char *msg)
{
    printf("PC: %04x; opcode: %04x; %s", chip8.pc, chip8.opcode, msg);
    exit(1);
}
