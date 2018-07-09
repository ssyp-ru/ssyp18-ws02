LD=ld
CC=gcc
CFLAGS=-std=c11 -Wall -Wpedantic -g -O0
LDLIBS=-lncurses
SOURCES=genmap.c shrew_map.c cellular_map.c\
            mapgen.c level.c actor.c main.c\
            levelvector.o kdtree.c roomvector.c behave.c

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
main_shrew: main_shrew.o shrew_map.o cellular_map.o genmap.o
main_cellular: main_cellular.o shrew_map.o cellular_map.o genmap.o
main_recdev: main_recdev.o genmap.o recursive_devision_map.o roomvector.o
GUI_test: GUI_test.o GUI.o feature.o find_path.o
kdtreetest: kdtreetest.o $(OBJECTS)	
gen_feature:  gen_feature.o kdtree.o, levelvector.o;
