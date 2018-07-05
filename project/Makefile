LINKER= ld
CC= gcc
CFLAGS= -std=c11 -Wall -Wpedantic -g -O0 
LDLIBS= -lncurses -lm
SOURCES= graphics.c generate.c actor.c
OBJECTS= $(SOURCES:.c=.o)
EXECUTABLE= main
main: $(OBJECTS)
all: $(OBJECTS) $(EXECUTABLE)
