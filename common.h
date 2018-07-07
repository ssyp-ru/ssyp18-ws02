#pragma once
#include <ncurses.h>

typedef struct room {
	int x, y;
	int height, width;
} room_t;

typedef struct inventory {
	char* name;
	unsigned int item_count;
} inv_t;

typedef struct actor {
	unsigned char health;
	int speed;
	unsigned char painchance;
	// if the bullet hits. Actor with any probability
	// runs away (painchance < 256)
	chtype symbol;
	unsigned int x, y;
	inv_t* inventory;
	unsigned char
	state;  // current state.For example monster search monster
	int flags;            // features of actor(maybe check actor is player)
	unsigned int targx,
	         targy;  //-|____position of target of this actor(player,
	//item, other actor)
	struct level* level;
	int (*behave)(struct actor*);
} actor_t;

typedef int (*behave_t)(struct actor*);

typedef struct actors_vect {
	struct actor** all_actors;
	unsigned int len;
	unsigned int capacity;
} avect_t;

typedef struct room_vector {
	room_t* data;
	size_t length;
	size_t capacity;
} room_vector_t;

typedef struct tile {
	chtype symbol;
	int flags;
} tile_t;

typedef struct map {
	tile_t* buffer;
	room_vector_t* rooms;
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
