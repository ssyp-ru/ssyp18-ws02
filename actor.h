#ifndef _ACTOR_05_07_18_11_21
#define _ACTOR_05_07_18_11_21

typedef struct actor{
	chtype symbol;
	int flags;
	int x, y;
} actor_t;


typedef struct actors{
	actor_t* array;
	int amount;
} actors_t;


typedef struct level{
	map_t* map;
	actors_t* actors;
} level_t;

