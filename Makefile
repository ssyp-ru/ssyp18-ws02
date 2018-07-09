LD=ld
CC=gcc
<<<<<<< HEAD
CFLAGS=-std=c11 -Wall -Wpedantic -g -O0 
LDLIBS=-lncurses -lm
LD=ld
SOURCES=genmap.c shrew_map.c cellular_map.c\
				level.c actor.c main.c kdtree.o collect.o\
				roomvector.c behave.c game.c levelvector.c GUI.c
OBJECTS=$(SOURCES:.c=.o)
all: main
build: all
run: all
	./main
clean:
	rm -rf *.o all
single: GUI.o game.o behave.o kdtree.o collect.o\
			actor.o level.o levelvector.o roomvector.o feature.o\
			genmap.o genmap_shrew.o genmap_cellular.o genmap_recdev.o

kdtreetest: kdtree.o kdtreetest.o collect.o
main_shrew: main_shrew.o shrew_map.o cellular_map.o genmap.o
main_cellular: main_cellular.o shrew_map.o cellular_map.o genmap.o
main_recdev: main_recdev.o genmap.o recursive_devision_map.o roomvector.o
GUI_test: GUI_test.o GUI.o feature.o find_path.o
kdtreetest: kdtreetest.o $(OBJECTS)	
gen_feature:  gen_feature.o kdtree.o levelvector.o collect.o;
