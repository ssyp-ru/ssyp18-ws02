LD=ld
CC=gcc
CFLAGS=-std=c11 -Wall -Wpedantic -g -O0
LDLIBS=-lncurses	
SOURCES=genmap.c shrew_map.c cellular_map.c
OBJECTS=$(SOURCES:.c=.o)

all: $(SOURCES)  $(EXECUTABLE)

build: all

run: all
	./$(EXECUTABLE)

clean:
	rm -rf *.o all
main_shrew: main_shrew.o shrew_map.o cellular_map.o genmap.o
main_cellular: main_cellular.o shrew_map.o cellular_map.o genmap.o

