#CFLAGS = -Werror -std=c11 -pedantic -ggdb3  -Wall -Wextra -g3
CC = clang
SHELL = /bin/bash

CFLAGS = -std=c11 -DNDEBUG -O2 -pedantic -Werror -Wall -Wextra -Isrc  $(OPTFLAGS)
#-Wl,--no-as-needed -ldl-rdynamic nur bei cc als compiler?
LIBS = -ldl $(OPTLIBS)
PREFIX? = /usr/local

SOURCES = $(wildcard src/**/*.c src/*.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

TEST_SRC = $(wildcard tests/*_test.c)
TESTS = $(patsubst %.c,%,$(TEST_SRC))

TARGET = build/liblcthw.a
SO_TARGET = $(patsubst %.a,%.so,$(TARGET))

all: $(TARGET) $(SO_TARGET) tests
	
#dev: CFLAGS = -std=c11 -g -pedantic -Werror -Wall -Wextra -Isrc $(OPTFLAGS)
#dev: all
	
$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@
$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

build:
	@mkdir -p build
	@mkdir -p bin
	   
# The Unit Tests
.PHONY: tests
tests: CFLAGS += $(SO_TARGET)
tests: CFLAGS := $(filter-out -DNDEBUG,$(CFLAGS))
tests: CFLAGS += -g3 -ggdb
tests: $(TESTS)
	$(SHELL) ./tests/runtests.sh
	   
valgrind:
	VALGRIND="valgrind --log-file=/tmp/valgrind-%p.log" $(MAKE)
	   
# The Cleaner
clean:
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f tests/tests.log
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`
	   
# The Install
install: all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/

# The Checker
BADFUNCS='[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)|stpn?cpy|a?sn?printf|byte_)'
check:
	@echo Files with potentially dangerous functions.
	@egrep $(BADFUNCS) $(SOURCES) || true

