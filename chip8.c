#include "chip8.h"

/* initializes chip8state to correct initial values */
void chip8init(chip8state *chip8)
{
    //TODO: finish initialization
    // setting all registers, memory back to 0
    // memset for V works, but memset for memory fails. why?
    memset(chip8->V, 0, 16 * sizeof(unsigned char));
    memset(chip8->memory, 0, 4096 * sizeof(unsigned char));
    memset(chip8->stack, 0, 16 * sizeof(unsigned short));
    memset(chip8->graphics, 0, 64*32*sizeof(unsigned char));
    chip8->I = 0;
    chip8->pc = 0x200;
    chip8->delay = 0;
    chip8->sound = 0;
    chip8->sp = 0;
    chip8->opcode = 0;
    
    // now need to load the fonts into 0x00-0x1FF
    for (int i = 0; i < 80; i++)
    {
        writeMem(i, chip8fonts[i], chip8);
    }

    // seeding rng for RND instruction
    srand(time(NULL));

    chip8->window = graphicsInit();
}

/* reads file into memory byte by byte then closes file */
void program(char *filename, chip8state *chip8)
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
        chip8->memory[pc] = (unsigned char) c;
        pc++;
    }
    fclose(fp);
}

/* reads next two chars from memory, then increments pc by 2 */
void fetch(chip8state *chip8)
{
    // left shifting higher address (MSB = leftmost bit) 
    chip8->opcode = (chip8->memory[chip8->pc] << 8) | (chip8->memory[chip8->pc + 1]);
    chip8->pc += 2;
}

/* executes the current instruction */
void execute(chip8state *chip8)
{
    unsigned char highnib = ((0xF000) & (chip8->opcode)) >> 12;
    unsigned char lownib = (0x000F) & (chip8->opcode);
    switch(highnib)
    {
        case(0):
            {
                switch(lownib)
                {
                    case(0):
                        {
                            // Clear screen
                            memset(chip8->graphics, 0, 64*32*sizeof(unsigned char));
                            // TODO: call draw screen function to clear this out
                            graphicsUpdate(chip8->graphics, chip8->window);
                            break;
                        }
                    case(0xE):
                        {
                            // Return from func
                            chip8->pc = chip8->stack[chip8->sp];
                            chip8->sp--;
                            break;
                        }
                    default:
                        unhandledInstruction("SYS addr", chip8);
                }
                break;
            }
        case(1):
            {
                // JP to nnn - set pc to 3 least sig digits
                chip8->pc = GET_NNN(chip8->opcode);
                break;
            }
        case(2):
            {
                // CALL addr
                chip8->sp++;
                chip8->stack[chip8->sp] = chip8->pc;
                chip8->pc = GET_NNN(chip8->opcode);
                break;
            }
        case(3):
            {
                // SE Vx, byte - pc += 2 if V[x] == kk
                unsigned int x = GET_X(chip8->opcode);
                unsigned char kk = GET_KK(chip8->opcode);
                if (readReg(x, chip8) == kk)
                {
                    chip8->pc += 2;
                }
                break;
            }
        case(4):
            {
                // SNE Vx, byte - pc += 2 if V[x] != kk
                unsigned int x = GET_X(chip8->opcode);
                unsigned char kk = GET_KK(chip8->opcode);
                if (readReg(x, chip8) != kk)
                {
                    chip8->pc += 2;
                }
                break;
            }
        case(5):
            {
                // SE Vx, Vy - pc += 2 if V[x} == V[y]
                unsigned int x = GET_X(chip8->opcode);
                unsigned int y = GET_Y(chip8->opcode);
                if (readReg(x, chip8) == readReg(y, chip8))
                {
                    chip8->pc += 2;
                }
                break;
            }
        case(6):
            {
                // LD Vx, byte - V[x] == kk
                unsigned int x = GET_X(chip8->opcode);
                unsigned char kk = GET_KK(chip8->opcode);
                writeReg(x, kk, chip8);
                break;
            }
        case(7):
            {
                // ADD Vx, byte - Vx = Vx + kk
                unsigned int x = GET_X(chip8->opcode);
                unsigned char kk = GET_KK(chip8->opcode);
                unsigned char val = readReg(x, chip8) + kk;
                writeReg(x, val, chip8);
                break;
            }
        case(8):
            {
                unsigned int x = GET_X(chip8->opcode);
                unsigned int y = GET_Y(chip8->opcode);
                switch (lownib)
                {
                  case(0):
                      {
                          // LD Vx, Vy - Vx = Vy
                          unsigned char val = readReg(y, chip8);
                          writeReg(x, val, chip8);
                          break;
                      }
                  case(1):
                      {
                          // OR Vx, Vy - Vx = Vx OR Vy
                          unsigned char val = readReg(y, chip8) | readReg(x, chip8);
                          writeReg(x, val, chip8);
                          break;
                      }
                  case(2):
                      {
                          // AND Vx, Vy - Vx = Vx AND Vy
                          unsigned char val = readReg(y, chip8) & readReg(x, chip8);
                          writeReg(x, val, chip8);
                          break;
                      }
                  case(3):
                      {
                          // XOR Vx, Vy - Vx = Vx XOR Vy
                          unsigned char val = readReg(y, chip8) ^ readReg(x, chip8);
                          writeReg(x, val, chip8);
                          break;
                      }
                  case(4):
                      {
                          // ADD Vx, Vy - Vx = Vx + Vy, VF = carry
                          unsigned char val = readReg(y, chip8) + readReg(x, chip8);
                          
                          // does this set VF correctly? how else can we check?
                          if (val < readReg(y, chip8) || val < readReg(x, chip8))
                          {
                              writeReg(0xF, 1, chip8);
                          }
                          else
                          {
                              writeReg(0xF, 0, chip8);
                          }
                          writeReg(x, val, chip8);
                          break;
                      }
                  case(5):
                      {
                          // SUB Vx, Vy - Vx = Vx - Vy, VF = NOT borrow
                          unsigned char val = readReg(x, chip8) - readReg(y, chip8);

                          // setting VF if necessary
                          if (readReg(x, chip8) > readReg(y, chip8))
                          {
                              writeReg(0xF, 1, chip8);
                          }
                          else
                          {
                              writeReg(0xF, 0, chip8);
                          }
                          writeReg(x, val, chip8);
                          break;
                      }
                  case(6):
                      {
                          // SHR Vx, Vx = Vx >> 1, VF = lsb of Vx
                          unsigned char val = readReg(x, chip8) >> 1;
                          
                          // checking lsb of Vx
                          if ((readReg(x, chip8) & 1) == 1)
                          {
                              writeReg(0xF, 1, chip8);
                          }
                          else
                          {
                              writeReg(0xF, 0, chip8);
                          }
                          writeReg(x, val, chip8);
                          break;
                      }
                  case(7):
                      {
                          // SUBN Vx, Vy - Vx = Vy - Vx
                          unsigned char val = readReg(y, chip8) - readReg(x, chip8);

                          // setting VF if necessary
                          if (readReg(y, chip8) > readReg(x, chip8))
                          {
                              writeReg(0xF, 1, chip8);
                          }
                          else
                          {
                              writeReg(0xF, 0, chip8);
                          }
                          writeReg(x, val, chip8);
                          break;                            
                      }
                  case(0xE):
                      {
                          // SHL Vx, Vx = Vx << 1
                          unsigned char val = readReg(x, chip8) << 1;

                          // VF stuff - is 0x80 correct? 
                          if ((readReg(x, chip8) & 0x80) == 0x80)
                          {
                              writeReg(0xF, 1, chip8);
                          }
                          else
                          {
                              writeReg(0xF, 0, chip8);
                          }
                          writeReg(x, val, chip8);
                          break;
                      }
                  default:
                      {
                          printError("Unknown mnemonic", chip8);
                          break;
                      }
              } 
              break;
            }
        case(9):
            {
                // SNE Vx, Vy - pc += 2 if Vx != vY
                unsigned int x = GET_X(chip8->opcode);
                unsigned int y = GET_Y(chip8->opcode);
                if (readReg(x, chip8) != readReg(y, chip8))
                {
                    chip8->pc += 2;
                }
                break;
            }
        case(0xA):
            {
                // LD I, addr
                chip8->I = GET_NNN(chip8->opcode);
                break;
            }
        case(0xB):
            {
                // JP V0, addr - pc = V[0] + nnn
                chip8->pc = readReg(0, chip8) + GET_NNN(chip8->opcode);
                break;
            }
        case(0xC):
            {
                // RND Vx, byte - set Vx = random byte & kk
                int x = GET_X(chip8->opcode);
                unsigned char val = ((unsigned char) rand()%256) & GET_KK(chip8->opcode); 
                writeReg(x, val, chip8);
                break;
            }
        case(0xD):
            {
                // DXYN - Draw Vx, Vy, nibble
                // reads n bytes from memory, starting at I and displays them
                // at (Vx, Vy)
                unsigned char byte;

                unsigned char Vx = readReg(GET_X(chip8->opcode), chip8);
                unsigned char Vy = readReg(GET_Y(chip8->opcode), chip8);

                unsigned char xpos = 0;
                
                // this part will update the graphics array
                // loop through memory to get each byte
                for (int i = chip8->I; i <= chip8->I + lownib; i++)
                {
                    // retrieve the byte
                    byte = readMem(i, chip8);

                    // put the byte into the graphics starting at (Vx, Vy)
                    for (int j = 0; j < 8; j++)
                    {
                        // checking for collision so we can set the VF flag
                        if ((chip8->graphics[Vx + j][Vy] == 1) && (((byte >> (8 - j)) & 1)  > 0))
                        {
                            writeReg(0xF, 1, chip8);
                        }

                        // checking if we need to wrap around
                        xpos = Vx + j;
                        if (xpos > SCREEN_WIDTH)
                        {
                            xpos = xpos - SCREEN_WIDTH;
                        }

                        // right shift by (8-j) times, then logical and with 1
                        chip8->graphics[xpos][Vy] ^= (byte >> (8 - j)) & 1;
                    }
                    
                    // I think Vy should increment here to start drawing to the
                    // next line
                    Vy++;
                    
                    // wrap around the screen
                    if (Vy > SCREEN_WIDTH)
                    {
                        Vy = 0;
                    }
                }
                
                // after we finish updating the graphics array, then we can 
                // update the screen
                graphicsUpdate(chip8->graphics, chip8->window);
                break;
            }
        case(0xE):
            {
                switch(lownib)
                {
                    case(0xE):
                        {
                            unhandledInstruction("SKP Vx", chip8);
                            break;
                        }
                    case(1):
                        {
                            unhandledInstruction("SKNP Vx", chip8);
                            break;
                        }
                    default:
                        {
                            printError("Unknown Mnemonic", chip8); 
                            break;
                        }
                }
                break;
            }
        case(0xF):
            {
                switch (chip8->opcode & 0x00FF)
                {
                    case(0x07):
                        {
                            // LD Vx, DT
                            int x = GET_X(chip8->opcode);
                            writeReg(x, chip8->delay, chip8);
                            break;
                        }
                    case(0x0A):
                        {
                            unhandledInstruction("LD Vx, K", chip8);
                            break;
                        }
                    case(0x15):
                        {
                            // LD DT, Vx
                            int x = GET_X(chip8->opcode);
                            chip8->delay = readReg(x, chip8);
                            break;
                        }
                    case(0x18):
                        {
                            // LD ST, Vx
                            int x = GET_X(chip8->opcode);
                            chip8->sound = readReg(x, chip8);
                            break;
                        }
                    case(0x1E):
                        {
                            // ADD I, Vx
                            int x = GET_X(chip8->opcode);
                            chip8->I = chip8->I + readReg(x, chip8);
                            break;
                        }
                    case(0x29):
                        {
                            unhandledInstruction("LD F, Vx", chip8);
                            break;
                        }
                    case(0x33):
                        {
                            unhandledInstruction("LD B, Vx", chip8);
                            break;
                        }
                    case(0x55):
                        {
                            // LD [I], Vx
                            int x = GET_X(chip8->opcode);
                            for (int i = chip8->I; i < (chip8->I+x); i++)
                            {
                                writeMem(i, readReg(i - chip8->I, chip8), chip8);
                            }
                            break;
                        }
                    case(0x65):
                        {
                            // LD Vx, [I]
                            int x = GET_X(chip8->opcode);
                            for (int i = chip8->I; i < (chip8->I+x); i++)
                            {
                                writeReg(i - chip8->I, readMem(i, chip8), chip8);
                            }
                            break;
                        }
                }
                break;
            }

        default:
            printError("Unknown mnemonic", chip8);
    }

}

/*********** Helper functions *********************************/
/* function to read from memory */
unsigned char readMem(int address, chip8state *chip8)
{
    DEBUG_PRINT("Opcode: %04x, reading from address %x\n", chip8->opcode, address);
    if (address >= 0 && address <= 4096)
    {
        return chip8->memory[address];
    }
    else
    {
        printError("Memory access error.", chip8);
        return -1;
    }
}

/* writes to specified memory address */
void writeMem(int address, unsigned char val, chip8state *chip8)
{
    DEBUG_PRINT("Opcode: %04x, writing val 0x%x to address 0x%x\n", chip8->opcode, val, address);
    if (address >= 0 && address <= 4096)
    {
        chip8->memory[address] = val;
    }
    else
    {
        printError("Memory access error.", chip8);
    }
}

/* reads from register Vx */
unsigned char readReg(int x, chip8state *chip8)
{
    DEBUG_PRINT("Opcode: %04x, reading from reg %x\n", chip8->opcode, x);
    if (x >= 0 && x <= 15)
    {
        return chip8->V[x];
    }
    else
    {
        printError("Register access errror.", chip8);
        return -1;
    }
}

/* writes value to register Vx */
void writeReg(int x, unsigned char val, chip8state *chip8)
{
    DEBUG_PRINT("Opcode: %04x, writing val 0x%x to reg 0x%x\n", chip8->opcode, val, x);
    if (x >= 0 && x <= 15)
    {
        chip8->V[x] = val;
    }
    else
    {
        printError("Register access error.", chip8);
    }
}

/* Prints out opcode of unhandled instruction */
void unhandledInstruction(char *msg, chip8state *chip8)
{
    printf("Unhandled instruction: %s; %04x\n", msg, chip8->opcode);
}

/* helper function, for debugging/testing */
void printMemory(int address, int period, chip8state *chip8)
{
    int count = 0;
    char c = 0;
    
    while (1)
    {
        printf("\nMemory locations from 0x%x to 0x%x\n", address, address + period);

        // printing out memory locations, 16 per line
        for (count = 0; count <= period; count++)
        {
            printf("%02x", chip8->memory[address + count]);
            
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

/* prints current opcode */
void printOpcode(chip8state *chip8)
{
    printf("%04x\n", chip8->opcode);
}

/* printing error msg w/ current pc, opcode */
void printError(char *msg, chip8state *chip8)
{
    fprintf(stderr, "PC: %04x; opcode: %04x; %s", chip8->pc, chip8->opcode, msg);
    exit(1);
}
