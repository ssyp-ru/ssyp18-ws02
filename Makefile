LD=ld
CC=gcc
CFLAGS=-std=c11 -Wall -Wpedantic -g -O0 
LDLIBS=-lncurses -lm
LD=ld
SOURCES=genmap.c net.c net_client.c log.c datastruct.c net_server.c\
				level.c actor.c main.c collect.c find_path.c\
				single.c roomvector.c behave.c genmap_recdev.c kdtree.c game.c levelvector.c GUI.c
OBJECTS=$(SOURCES:.c=.o)
all: main
#SOURCES=main.c net_client.c net_server.c net.c log.c datastruct.c
#EXECUTABLE=serv

all: $(SOURCES) $(EXECUTABLE)

build: all
run: all
	./main
clean:
	rm -rf *.o all
single: GUI.o game.o behave.o kdtree.o collect.o\
			actor.o level.o levelvector.o roomvector.o feature.o\
			genmap.o find_path.o genmap_shrew.o genmap_cellular.o genmap_recdev.o
client: $(OBJECTS)



kdtreetest: kdtree.o kdtreetest.o collect.o
