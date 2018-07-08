CC=gcc

CFLAGS=-std=c11 -Wall -Wpedantic -g -O0 
LDLIBS=-lncurses
LD=ld
SOURCES=genmap.c shrew_map.c cellular_map.c\
				level.c actor.c main.c\
				roomvector.c behave.c game.c levelvector.c GUI.c
OBJECTS=$(SOURCES:.c=.o)
all: main
build: all
run: all
	./main
clean:
	rm -rf *.o all
single: GUI.o game.o behave.o\
			actor.o level.o levelvector.o roomvector.o feature.o\
			genmap.o genmap_shrew.o genmap_cellular.o genmap_recdev.o
