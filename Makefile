LD=ld
CC=gcc
CFLAGS=-std=c11 -Wall -Wpedantic -g -O0 -Werror
LDLIBS=-lncurses -lm

GUI_test: GUI_test.o GUI.o feature.o find_path.o genmap.o\
					shrew_map.o roomvector.o
SOURCES=genmap.c shrew_map.c cellular_map.c\
				mapgen.c level.c actor.c main.c\
				roomvector.c behave.c 
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

kdtreetest: kdtreetest.o $(OBJECTS)	
