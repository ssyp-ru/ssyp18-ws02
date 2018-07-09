LD=ld
CC=gcc
CFLAGS=-std=c11 -Wall -Wpedantic -g -O0 
LDLIBS=-lncurses -lm
LD=ld
SOURCES=genmap.c genmap_cellular.c genmap_shrew.c\
				genmap_recdev.c genmap_tree.c\
				level.c actor.c main.c kdtree.o collect.o\
				roomvector.c behave.c game.c levelvector.c GUI.c\
			 	rooms.c room_tree.c vector_tree.c
OBJECTS=$(SOURCES:.c=.o)
all: main
build: all
run: all
	./main
clean:
	rm -rf *.o all
single: GUI.o game.o behave.o kdtree.o collect.o\
			actor.o level.o levelvector.o roomvector.o feature.o\
			genmap.o genmap_shrew.o genmap_cellular.o genmap_recdev.o\
			genmap_tree.o rooms.o room_tree.o vector_tree.o

kdtreetest: kdtree.o kdtreetest.o collect.o
