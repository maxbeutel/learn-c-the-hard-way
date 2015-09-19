SHELL=/bin/bash
CC=clang-3.5
CFLAGS=-std=c11 -pedantic -ggdb3 -Werror -Wall -g3

all: clean ex17
	
run:
	./ex17 db.dat c
	./ex17 db.dat s 0 max max@zedshaw.com
	./ex17 db.dat s 1 zed zed@zedshaw.com
	./ex17 db.dat s 2 dud dud@zedshaw.com
	./ex17 db.dat s 3 joe joe@zedshaw.com
	#./ex17 db.dat l
	#./ex17 db.dat d 3
	#./ex17 db.dat l
	#./ex17 db.dat g 2
	#./ex17 db.dat l

.PHONY: run

clean:
	rm -f ./ex17
	rm -f ./db.dat