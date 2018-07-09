#pragma once

#include <ncurses.h>

#define FLAG_TILE_SOLID         0x00000001
#define FLAG_TILE_TRANSPARENT   0x00000002
#define FLAG_TILE_DESTRUCTIBLE  0x00000004

#define FLAG_FEATURE_PERSISTENT 0x00000001
#define FLAG_FEATURE_EDIBILITY  0x00000002
#define FLAG_FEATURE_CLOTHES    0x00000004
#define FLAG_FEATURE_WEAPON     0x00000008

typedef struct room {
	int x, y;
	int height, width;
} room_t;

typedef struct room_vector {
	room_t* data;
	size_t length;
	size_t capacity;
} rooms_vt;

//--------------------------

#define FLAG_ITEM_STACKABLE 0x1
typedef struct item {
	chtype type;
	float weight;
	int flags;
	char * description;
	int quality;
	int amount;
} item_t;

typedef struct inventory{
	item_t * data;
	size_t length;
	size_t capacity;
} inventory_t;

//--------------------------

typedef struct tile{
	chtype symbol;
	int flags;
} tile_t;

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

//--------------------------

typedef struct actor{
	unsigned int id;
	char * name;
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

typedef struct actors_vect {
	struct actor** data;
	size_t length;
	size_t capacity;
} actors_vt;


//--------------------------

typedef enum feature_type {
	NOTHING, DROPPED_ITEM, CONTAINER,
	DOOR, DEAD_BADGER, STAIRS
} ftype_t;

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

//--------------------------

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

//--------------------------

typedef struct box{
	int x, y;
	int width, height;
} box_t;

