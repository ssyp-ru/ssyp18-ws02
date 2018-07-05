LD=ld
CC=gcc
CFLAGS=-std=c11 -Wall -Wpedantic -g -c -O0
LDLIBS=-lncurses	
SOURCES=main.c caves.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

all: $(SOURCES)  $(EXECUTABLE)

build: all

run: all
	./$(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LDLIBS)
clean:
	rm -rf *.o all
