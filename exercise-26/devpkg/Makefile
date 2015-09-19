PREFIX ?= /usr/local
CFLAGS = -Werror -std=c11 -pedantic -Wall -Wextra -ggdb3 -g3 -I${PREFIX}/apr/include/apr-1 -I${PREFIX}/apr/include/apr-util-1
LDFLAGS = -L${PREFIX}/apr/lib -lapr-1 -pthread -laprutil-1
#LDFLAGS= -L${PREFIX}/apr/lib -L .aprutil-1 -L .apr-1
LDLIBS=  -lapr-1 -laprutil-1

all: devpkg

run_db: db.o bstrlib.o 

run_shell: shell.o 

run_commands: bstrlib.o db.o shell.o commands.o

devpkg: bstrlib.o db.o shell.o commands.o
	
install: all
	install -d $(DESTDIR)/$(PREFIX)/bin/
	install devpkg $(DESTDIR)/$(PREFIX)/bin/
	
clean:
	rm -f *.o
	rm -f devpkg
	rm -rf *.dSYM
