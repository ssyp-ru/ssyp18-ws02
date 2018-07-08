#pragma once
typedef struct position{
	int x,y;
} pos_t;

typedef struct point_vector_t{
	pos_t * buffer;
	size_t length;
	size_t capacity;
} pvector_t;

//Find a path from actor to a position
pvector_t * find_path(actor_t actor, int x, int y);


