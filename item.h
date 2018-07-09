#pragma once
#include <ncurses.h>
#define FLAG_STACK 0x1

typedef struct item(){
	chtype look;
	int xPos;
	int yPos;
}item_t

