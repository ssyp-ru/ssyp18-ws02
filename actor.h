#pragma once
#include <ncurses.h>
#define FLAG_FRIENDLY 0x1
#define FLAG_STACKABLE 0x1
#include "common.h"
/*
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
=======*/
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "behave.h"
#include "mapgen.h"

void draw_actors(avect_t* vect);

avect_t* init_actors(level_t* level,
                     int amount_of_entities);

avect_t* create_new_vector(int num);

void add_vector_elem(avect_t* vect, actor_t* newActor);

actor_t* actor_get(avect_t* vect, int num);

void free_actors(avect_t* vect, bool isFull);
//>>>>>>> 5ad922adbebaa1fdeec05390abd28df28a613504


