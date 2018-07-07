CC=gcc
CFLAGS=-std=c11 -Wall -Wpedantic -g -O0
LDLIBS=-lncurses

GUI_test: GUI_test.o GUI.o feature.o
