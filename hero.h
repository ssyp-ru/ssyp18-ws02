#pragma once
#include <ncurses.h>

typedef struct vision{
	int xPos;
	int yPos;
} vision_t;
typedef struct inventory{
	chtype type;
	float weight;
	char * description;
	int quality;
}inventory_t; 
typedef struct hero{
	vision_t * view;
	inventory_t * inventory;
	int hp;
	int viewRad;
	int xPos;
	int yPos;
	chtype look;
} hero_t;	


