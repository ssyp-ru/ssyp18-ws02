LD=ld
CC=gcc
CFLAGS=-std=c11 -Wall -Wpedantic -g -O0 -Werror
LDLIBS=-lncurses -lm
SOURCES=main.c 
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

build: all

kdtreetest: kdtree.o kdtreetest.o

run: all
	./$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LDLIBS)

clean:
	rm -rf *.o all
	
