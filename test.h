#ifndef TEST_H
#define TEST_H
#include "chip8.h"

// only function that should be called
void test(chip8state *chip8);

// individual test functions for each instruction - all return -1 on failure, 
// return 1 on success
int test1nnn(chip8state *chip8);
int test2nnn(chip8state *chip8);
int test3xkk(chip8state *chip8);
int test4xkk(chip8state *chip8);
int test5xy0(chip8state *chip8);
int test6xkk(chip8state *chip8);
int test7xkk(chip8state *chip8);
int test8xy0(chip8state *chip8);
int test8xy1(chip8state *chip8);
int test8xy2(chip8state *chip8);
int test8xy3(chip8state *chip8);
int test8xy4(chip8state *chip8);
int test8xy5(chip8state *chip8);
int test8xy6(chip8state *chip8);
int test8xy7(chip8state *chip8);
int test8xyE(chip8state *chip8);
int test9xy0(chip8state *chip8);
int testAnnn(chip8state *chip8);
int testBnnn(chip8state *chip8);
int testCxkk(chip8state *chip8);
int testDxyn(chip8state *chip8);
int testEx9E(chip8state *chip8);
int testExA1(chip8state *chip8);
int testFx07(chip8state *chip8);
int testFx0A(chip8state *chip8);
int testFx15(chip8state *chip8);
int testFx18(chip8state *chip8);
int testFx1E(chip8state *chip8);
int testFx29(chip8state *chip8);
int testFx33(chip8state *chip8);
int testFx55(chip8state *chip8);
int testFx65(chip8state *chip8);
#endif
