#pragma once

#include <ncurses.h>

#define FLAG_SOLID 0x1
#define FLAG_TRANS 0x2
#define FLAG_DESTR 0x4

typedef struct room{
	int x, y;
	int height, width;
} room_t;

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
	unsigned int id;
	int hp;
	int direct;
	chtype symbol; 
	int x, y;
	int strength;
	int agility;
	int stamina;
	int view_radius;
	struct inventory* inventory;
	unsigned int state;
	int flags;				
	int targ_x, targ_y;
	struct level  *level;
	int (*behave)(struct actor * );
	int* memory;	
} actor_t;

typedef int (*behave_t) (struct actor *);

typedef enum feature_type {
	NOTHING, DROPPED_ITEM, CONTAINER,
	DOOR, DEAD_BADGER, STAIR
} ftype_t;

typedef struct tile{
	chtype symbol;
  int flags;
} tile_t;

typedef struct feature{
	struct inventory * inventory;
	ftype_t type;
	int x, y;
	int flags;
	char * description;
	chtype symbol;
} feature_t;

typedef int (*behave_t)(struct actor*);

typedef struct actors_vect {
	struct actor* all_actors;
	unsigned int len;
	unsigned int capacity;
} avect_t;

typedef struct roomVector{
	room_t * data;
	size_t length;
	size_t capacity;
} room_vector_t;

typedef struct map{
	tile_t * buffer;
	int height;
 	int width;
	room_vector_t* rooms;
} map_t;

typedef struct level{
	map_t * map;
	avect_t * actors;
} level_t;
