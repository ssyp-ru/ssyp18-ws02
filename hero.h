#pragma once
#include <ncurses.h>
#define FLAG_FRIENDLY 0x1
#define FLAG_STACKABLE 0x1
typedef struct item {
	chtype type;
	float weight;
	int flags;
	char * description;
	int quality;
} item_t;

typedef struct inventory{
	item_t * item;
	int amount;
	int maxAmount;
} inventory_t;

typedef struct hero{
	inventory_t inventory;
	int flags;
	int hp;
	int strength;
	int agility;
	int stamina;
	int viewRad;
	int xPos;
	int yPos;
//	void (*behave)() = &behave;
	chtype look;
} hero_t;	


