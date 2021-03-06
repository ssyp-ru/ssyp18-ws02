LD=ld
CC=gcc
CFLAGS=-std=c11 -Wall -Wpedantic -g -O0 -Werror
LDLIBS=-lncurses -lm
LD=ld
SOURCES=genmap.c genmap_cellular.c genmap_shrew.c\
				genmap_recdev.c genmap_tree.c\
				level.c actor.c main.c kdtree.o collect.o\
				roomvector.c behave.c game.c levelvector.c GUI.c gen_feature.c\
				feature_prototypes.c feature.c
OBJECTS=$(SOURCES:.c=.o)
all: main
build: all
run: all
	./main
clean:
	rm -rf *.o all
single: GUI.o game.o behave.o kdtree.o collect.o\
			actor.o level.o levelvector.o roomvector.o feature.o\
			genmap.o find_path.o genmap_shrew.o genmap_cellular.o genmap_recdev.o feature_prototypes.o
