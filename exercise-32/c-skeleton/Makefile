CC = clang
SHELL = /bin/bash

CFLAGS = -std=c11 -O2 -pedantic -Werror -Wall -Wextra -Isrc  $(OPTFLAGS)

LIBS = -ldl $(OPTLIBS)
PREFIX? = /usr/local

SOURCES = $(wildcard src/**/*.c src/*.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

TEST_SRC = $(wildcard tests/*_test.c)
TESTS = $(patsubst %.c,%,$(TEST_SRC))

TARGET = build/liblcthw.a
SO_TARGET = $(patsubst %.a,%.so,$(TARGET))

all: $(TARGET) $(SO_TARGET)

$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@
$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

build:
	@mkdir -p build
	@mkdir -p bin


.PHONY: dev
dev: CFLAGS += -g3 -ggdb
dev: clean all tests

# The Unit Tests
.PHONY: tests
tests: CFLAGS += $(SO_TARGET)
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
