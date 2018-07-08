#pragma once
#include <ncurses.h>
#define FLAG_FRIENDLY 0x1
#define FLAG_STACKABLE 0x1
#include "common.h"

typedef struct item {
	chtype type;
	float weight;
	int flags;
	char * description;
	int quality;
	int amount;
} item_t;

typedef struct inventory{
	item_t * item;
	int amount;
	int max_amount;
} inventory_t;

typedef struct actor{
	struct inventory * inventory;
	int flags;
	int hp;
	int state;
	int targ_x, targ_y;
	int strength;
	int agility;
	int stamina;
	int view_radius;
	int x, y;
	struct level * level;
	int (*begave)(struct actor*);
	chtype symbol;
} actor_t;	


