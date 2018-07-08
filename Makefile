CC=gcc
LD=ld
CFLAGS=-std=c11 -Wall -Wpedantic -g -O0
LDLIBS=-lncurses
SOURCES=mapgen.c level.c actor.c main.c roomvector.c behave.c
OBJECTS=$(SOURCES:.c=.o)

all: $(OBJECTS)

main: mapgen.o level.o actor.o main.o roomvector.o behave.o
