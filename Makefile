CC=gcc
<<<<<<< HEAD
CFLAGS=-std=c11 -Wall -Wpedantic -g -O0
LDLIBS=-lncurses

GUI_test: GUI_test.o GUI.o feature.o
=======
LD=ld
CFLAGS=-std=c11 -Wall -Wpedantic -g -c -O0
LDLIBS=-lncurses
SOURCES=mapgen.c level.c actor.c main.c roomvector.c behave.c
EXECUTABLE=main
OBJECTS=$(SOURCES:.c=.o)

all: $(SOURCES) $(EXECUTABLE)

build: all

run: all
	./$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDLIBS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS) 

clean:
	rm -rf *.o all -rf
>>>>>>> 9720ab0e54903b10446cba34b9cfb3511273ce23
