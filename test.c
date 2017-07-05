#include "test.h"

void test(chip8state *chip8)
{
    printf("1nnn\n");
    if (!test1nnn(chip8)) { fprintf(stderr, "1nnn test failed!\n"); exit(1); }
    
    printf("3xkk\n");
    if (!test3xkk(chip8)) { fprintf(stderr, "3xkk test failed!\n"); exit(1); }

    printf("4xkk\n");
    if (!test4xkk(chip8)) { fprintf(stderr, "4xkk test failed!\n"); exit(1); }

    printf("5xy0\n");
    if (!test5xy0(chip8)) { fprintf(stderr, "5xy0 test failed!\n"); exit(1); }

    printf("6xkk\n");
    if (!test6xkk(chip8)) { fprintf(stderr, "6xkk test failed!\n"); exit(1); }

    printf("7xkk\n");
    if (!test7xkk(chip8)) { fprintf(stderr, "7xkk test failed!\n"); exit(1); }

    printf("8xy0\n");
    if (!test8xy0(chip8)) { fprintf(stderr, "8xy0 test failed!\n"); exit(1); }

    printf("8xy1\n");
    if (!test8xy1(chip8)) { fprintf(stderr, "8xy1 test failed!\n"); exit(1); }

    printf("8xy2\n");
    if (!test8xy2(chip8)) { fprintf(stderr, "8xy2 test failed!\n"); exit(1); }

    printf("8xy3\n");
    if (!test8xy3(chip8)) { fprintf(stderr, "8xy3 test failed!\n"); exit(1); }

    printf("8xy4\n");
    if (!test8xy4(chip8)) { fprintf(stderr, "8xy4 test failed!\n"); exit(1); }

    printf("8xy5\n");
    if (!test8xy5(chip8)) { fprintf(stderr, "8xy5 test failed!\n"); exit(1); }

    printf("8xy6\n");
    if (!test8xy6(chip8)) { fprintf(stderr, "8xy6 test failed!\n"); exit(1); }

    printf("8xy7\n");
    if (!test8xy7(chip8)) { fprintf(stderr, "8xy7 test failed!\n"); exit(1); }

    printf("8xyE\n");
    if (!test8xyE(chip8)) { fprintf(stderr, "8xyE test failed!\n"); exit(1); }
    printf("All tests pass!\n");
}

/****************** Test funcs for individual instructions ********************/
/* 1nnn - JP addr */
int test1nnn(chip8state *chip8)
{
    chip8init(chip8);
    // I'm not sure if it's a good idea to directly set the opcode, but i'm 
    // doing it anyway
    chip8->opcode = 0x1FFF;
    execute(chip8);
    return (chip8->pc == 0xFFF);
}

//TODO
/* 2nnn - CALL addr */
int test2nnn(chip8state *chip8)
{
    chip8init(chip8);
    //TODO still have to actually implement the stack correctly
    // chip8->stack[0] = 0;
    return 0;
}

/* SE Vx, byte */
int test3xkk(chip8state *chip8)
{
    int ret;
    chip8init(chip8);
    chip8->opcode = 0x30FF;
   
    // test 1: V[x] = kk
    writeReg(0, 0xFF, chip8);
    execute(chip8);
    ret = (chip8->pc == 0x202);

    // test 2: V[x] != kk
    writeReg(0, 0xFE, chip8);
    execute(chip8);
    return (ret && (chip8->pc == 0x202));
}

/* 4xkk - SNE Vx, byte */
int test4xkk(chip8state *chip8)
{
    int ret;
    chip8init(chip8);
    chip8->opcode = 0x4111;

    // test 1 - V[x] == kk
    writeReg(1, 0x11, chip8);
    execute(chip8);
    ret = (chip8->pc == 0x200);

    // test 2 - V[x] != kk
    writeReg(1, 0x23, chip8);
    execute(chip8);
    return (ret && (chip8->pc == 0x202));
}

/* 5xy0 - SE Vx, Vy */
int test5xy0(chip8state *chip8)
{
    int ret;
    chip8init(chip8);
    chip8->opcode = 0x5010;

    // case 1 - Vx = Vy
    writeReg(0, 0xFF, chip8);
    writeReg(1, 0xFF, chip8);
    execute(chip8);
    ret = (chip8->pc == 0x202);
   

    // case 2 - Vx != Vy
    writeReg(0, 0xFA, chip8);
    execute(chip8);
    return (ret & (chip8->pc == 0x202));
    
}

/* 6xkk - LD Vx, byte */
int test6xkk(chip8state *chip8)
{
    chip8init(chip8);
    chip8->opcode = 0x6A12;
    execute(chip8);
    return (readReg(0xA, chip8) == 0x12);
}


/* 7xkk - ADD Vx, byte */
int test7xkk(chip8state *chip8)
{
    chip8init(chip8);
    chip8->opcode = 0x7325;
    writeReg(3, 0x10, chip8);
    execute(chip8);
    return readReg(3, chip8) == 0x35;
}

/* 8xy0 - LD Vx, Vy */
int test8xy0(chip8state *chip8)
{
    chip8init(chip8);
    chip8->opcode = 0x8010;
    writeReg(0, 0xFF, chip8);
    writeReg(1, 0x10, chip8);
    execute(chip8);
    return (readReg(0, chip8) == 0x10);
}

/* 8xy1 - OR Vx, Vy */
int test8xy1(chip8state *chip8)
{
    chip8init(chip8);
    chip8->opcode = 0x8AB1;
    writeReg(0xA, 0xF0, chip8);
    writeReg(0xB, 0x0F, chip8);
    execute(chip8);
    return (readReg(0xA, chip8) == 0xFF);
}

/* 8xy2 - AND Vx, Vy */
int test8xy2(chip8state *chip8)
{
    chip8init(chip8);
    chip8->opcode = 0x8012;
    writeReg(0, 0xF0, chip8);
    writeReg(1, 0x0F, chip8);
    execute(chip8);
    return (readReg(0, chip8) == 0);
}

/* 8xy3 - XOR Vx, Vy */
int test8xy3(chip8state *chip8)
{
    chip8init(chip8);
    chip8->opcode = 0x8013;
    writeReg(0, 0xF3, chip8);
    writeReg(1, 0x0F, chip8);
    execute(chip8);
    return (readReg(0, chip8) == (0xF3 ^ 0x0F));
}

/* 8xy4 - ADD Vx, Vy */
int test8xy4(chip8state *chip8)
{
    int ret;
    chip8init(chip8);
    chip8->opcode = 0x8014;
    
    // case 1 - no overflow
    writeReg(0, 1, chip8);
    writeReg(1, 2, chip8);
    execute(chip8);
    ret = (readReg(0, chip8) == 3) && (readReg(0xF, chip8) == 0);

    // case 2 - overflow
    writeReg(0, 0xFF, chip8);
    writeReg(1, 2, chip8);
    execute(chip8);
    return ret && (readReg(0, chip8) == 1) && (readReg(0xF, chip8) == 1);
}
    
/* 8xy5 - SUB Vx, Vy */
int test8xy5(chip8state *chip8)
{
    int ret;
    chip8init(chip8);
    chip8->opcode = 0x8015;
    
    // case 1 - no borrow
    writeReg(0, 2, chip8);
    writeReg(1, 1, chip8);
    execute(chip8);
    ret = (readReg(0, chip8) == 1) && (readReg(0xF, chip8) == 1);

    // case 2 - borrow
    writeReg(0, 1, chip8);
    writeReg(1, 2, chip8);
    execute(chip8);
    return ret && (readReg(0, chip8) == 0xFF) && (readReg(0xF, chip8) == 0);
}

/* 8xy6 - SHR Vx */
int test8xy6(chip8state *chip8)
{
    int ret;
    chip8init(chip8);
    chip8->opcode = 0x8016;

    // case 1 - VF = 0
    writeReg(0, 0xF0, chip8);
    execute(chip8);
    ret = (readReg(0, chip8) == (unsigned char) (0xF0 / 2)) && (readReg(0xF, chip8) == 0);

    // case 2 - VF = 1
    writeReg(0, 1, chip8);
    execute(chip8);
    return ret && (readReg(0, chip8) == 0) && (readReg(0xF, chip8) == 1);
}

/* 8xy7 - SUBN Vx, Vy */
int test8xy7(chip8state *chip8)
{
    int ret;
    chip8init(chip8);
    chip8->opcode = 0x8017;

    // case 1 - no borrow
    writeReg(0, 7, chip8);
    writeReg(1, 8, chip8);
    execute(chip8);
    ret = (readReg(0, chip8) == 1) && (readReg(0xF, chip8) == 1);

    // case 2 - w/ borrow
    writeReg(0, 8, chip8);
    writeReg(1, 7, chip8);
    execute(chip8);
    return ret && (readReg(0, chip8) == 0xFF) && (readReg(0xF, chip8) == 0);
}

/* 8xyE - SHL Vx */
int test8xyE(chip8state *chip8)
{
    int ret;
    chip8init(chip8);
    chip8->opcode = 0x801E;

    // case 1 - VF = 0
    writeReg(0, 1, chip8);
    execute(chip8);
    ret = (readReg(0, chip8) == 2) && (readReg(0xF, chip8) == 0);

    // case 2 - VF = 1
    writeReg(0, 0xF0, chip8);
    execute(chip8);
    return ret && (readReg(0, chip8) == 0xE0) && (readReg(0xF, chip8) == 1);
}
