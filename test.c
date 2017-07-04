#include "test.h"

void test(chip8state *chip8)
{
    return;
}

/****************** Test funcs for individual instructions ********************/
/* 1nnn - JP addr */
int test1nnn(chip8state *chip8)
{
    chip8->pc = 0x200;
    // I'm not sure if it's a good idea to directly set the opcode, but i'm 
    // doing it anyway
    chip8->opcode = 0x1FFF;
    execute(chip8);
    if (chip8->pc != 0xFFF)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

//TODO
/* 2nnn - CALL addr */
int test2nnn(chip8state *chip8)
{
    chip8->pc = 0x200;
    chip8->sp = 0;
    //TODO still have to actually implement the stack correctly
    // chip8->stack[0] = 0;
    return 0;
}

/* SE Vx, byte */
int test3xkk(chip8state *chip8)
{
    chip8->pc = 0x200;
    chip8->opcode = 0x30FF;
   
    // test 1: V[x] = kk
    writeReg(0, 0xFF, chip8);
    execute(chip8);
    if (chip8->pc != 0x202)
    {
        return -1;
    }

    // test 2: V[x] != kk
    writeReg(0, 0xFE, chip8);
    execute(chip8);
    if (chip8->pc != 0x202)
    {
        return -1;
    }

    // tests pass!
    return 1;
}

/* 4xkk - SNE Vx, byte */
int test4xkk(chip8state *chip8)
{
    chip8->pc = 0x200;
    chip8->opcode = 0x4111;

    // test 1 - V[x] == kk
    writeReg(1, 0x11, chip8);
    execute(chip8);
    if (chip8->pc != 0x200)
    {
        return -1;
    }

    // test 2 - V[x] != kk
    writeReg(1, 0x23, chip8);
    execute(chip8);
    if (chip8->pc != 0x202)
    {
        return -1;
    }

    // tests pass!
    return 1;
}
