#pragma once

#include <ncurses.h>
#define FLAG_SOLID 0x1
#define FLAG_TRANS 0x2
#define FLAG_DESTR 0x4

typedef enum feature_type {
	NOTHING, DROPPED_ITEM, CONTAINER,
	DOOR, DEAD_BADGER, STAIR
} ftype_t;

typedef struct feature{
	struct inventory * inventory;
	ftype_t type;
	int x, y;
	int flags;
	char * description;
	chtype symbol;
} feature_t;

typedef struct tile{
	chtype symbol;
	int flags;
} tile_t;

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


#define UNPACK(varname, map_ptr) tile_t (*varname)[(map_ptr)->width] = (tile_t (*)[(map_ptr)->width]) map_ptr->buffer 

/**
 * This macros defines a new variable which allows to access map_t.buffer
 * as a two-dimensional array with width of map_t.width.
 * 
 * Example:
 * void map_foo(map_t* map_, int x, int y) {
 * 		UNPACK(map, map_);
 * 		if(map[x][y] == '@') {
 * 			...
 * 		}
 * }
*/

typedef int (*behave_t)(struct actor*);

typedef struct actors_vect {
	struct actor** all_actors;
	unsigned int len;
	unsigned int capacity;
} avect_t;

typedef struct room {
	int x, y;
	int height, width;
} room_t;


typedef struct room_vector {
	room_t* data;
	size_t length;
	size_t capacity;
} room_vector_t;


typedef struct map {
	tile_t* buffer;
	room_vector_t * rooms;
	int height, width;
} map_t;

typedef struct level {
	map_t* map;
} level_t;

typedef struct level_vector {
	level_t** data;
	size_t length;
	size_t capacity;
} level_vector_t;

