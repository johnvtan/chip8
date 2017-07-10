OBJS = main.c chip8.c graphics.c test.c
CC = gcc
CFLAGS = -std=gnu99 -Wall -w
LFLAGS = -lSDL2
OBJ_NAME = chip8

main: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LFLAGS) $(DEBUG) -o $(OBJ_NAME) 

debug: DEBUG = -g -DDEBUG

debug: main

clean:
	\rm *~ chip8 chip8test
