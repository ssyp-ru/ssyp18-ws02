CC=gcc
LD=ld
CFLAGS=-std=c11 -Wall -Wpedantic -g -c -O0
LDLIBS=-lncurses
SOURCES=serv.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=serv

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
