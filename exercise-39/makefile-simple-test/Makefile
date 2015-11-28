all: clean library main run

library:
	clang -c -std=c11 -Wall -Wextra -Werror -pedantic library.c

main:
	clang -std=c11 -Wall -Wextra -Werror -pedantic -o main library.o main.c

run:
	./main

clean:
	rm *.o
