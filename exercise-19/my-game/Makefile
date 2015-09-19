SHELL=/bin/bash
CC=clang-3.5
CFLAGS=-std=c11 -pedantic -ggdb3 -Werror -Wall -g3

all: clean main

main: room.o map.o

clean:
	rm -f ./main
	rm -f ./room.o
	rm -f ./map.o