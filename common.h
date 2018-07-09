#pragma once

#include <ncurses.h>
#define FLAG_SOLID 0x1
#define FLAG_TRANS 0x2
#define FLAG_DESTR 0x4

#define FLAG_FEATURE_PERSISTENT 0x1
#define FLAG_FEATURE_EDIBILITY  0x2
#define FLAG_FEATURE_CLOTHES    0x4
#define FLAG_FEATURE_WEAPON     0x8

typedef struct item {
	chtype type;
	float weight;
	int flags;
	char * description;
	int quality;
	int amount;
} item_t;

typedef enum feature_type {
	NOTHING, DROPPED_ITEM, CONTAINER,
	DOOR, DEAD_BADGER, STAIR
} ftype_t;

typedef struct tile{
	chtype symbol;
	int flags;
} tile_t;

typedef struct inventory{
	item_t * data;
	size_t length;
	size_t capacity;
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
	int (*behave)(struct actor*);
	chtype symbol;
} actor_t;	

typedef struct feature{
	struct inventory * inventory;
	ftype_t type;
	int x, y;
	int flags;
	char * description;
	chtype symbol;
	struct level * level;
	struct feature * crutch;
	void (*interact)(struct feature *, struct actor *);
	int fid;
} feature_t;

typedef struct features_vec {
	feature_t ** data;
	size_t size;
  size_t capacity;
} features_vt;

typedef struct kdtree {
	struct kdtree * rbranch;
	struct kdtree * lbranch;
	feature_t * node;
} kdtree_t;

typedef struct actors_vect {
	struct actor** data;
	size_t length;
	size_t capacity;
} actors_vt;

typedef struct room {
	int x, y;
	int height, width;
} room_t;

typedef struct room_vector {
	room_t* data;
	size_t length;
	size_t capacity;
} rooms_vt;


typedef struct map {
	tile_t* buffer;
	rooms_vt * rooms;
	int height, width;
} map_t;
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
typedef struct level {
	map_t* map;
  struct actors_vect * actors;
  kdtree_t * features; 
} level_t;

typedef struct level_vector {
	level_t** data;
	size_t length;
	size_t capacity;
} levels_vt;

typedef struct box{
	int x, y;
	int width, height;
} box_t;


