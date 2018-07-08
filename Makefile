CC=gcc

CFLAGS=-std=c11 -Wall -Wpedantic -g -O0 
LDLIBS=-lncurses
LD=ld
SOURCES=genmap.c shrew_map.c cellular_map.c\
				mapgen.c level.c actor.c main.c\
				roomvector.c behave.c game.c levelvector.c GUI.c
OBJECTS=$(SOURCES:.c=.o)
all: main
build: all
run: all
	./main
clean:
	rm -rf *.o all
main: GUI.o actor.o genmap.o shrew_map.o roomvector.o feature.o game.o\
			levelvector.o behave.o level.o mapgen.o
main_shrew: main_shrew.o shrew_map.o cellular_map.o genmap.o
main_cellular: main_cellular.o shrew_map.o cellular_map.o genmap.o
main_recdev: main_recdev.o genmap.o recursive_devision_map.o roomvector.o
