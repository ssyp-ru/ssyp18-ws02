LD=ld
CC=gcc
CFLAGS=-std=c11 -Wall -Wpedantic -g -O0 -Werror
lab: tree.c
LDLIBS=-lncurses -lm
SOURCES= kdtree.c collect.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

build: all

run: all
	./$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LDLIBS)

clean:
	rm -rf *.o all

kdtreetest: kdtreetest.o $(OBJECTS)	
