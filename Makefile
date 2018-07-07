LD=ld
CC=gcc
CFLAGS=-std=c11 -Wall -Wpedantic -c -g -O0 -Werror
lab: tree.c
LDLIBS=-lncurses
SOURCES=main.c tree.c
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
	
