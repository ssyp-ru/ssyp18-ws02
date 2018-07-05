#pragma once

#include "room.h"
#include <ncurses.h>

typedef struct roomVector{
	room_t* data;
	size_t length;
	size_t capacity;
} roomVector_t;


roomVector_t* vector_init(int L);


room_t vector_get(roomVector_t* v, int i);


void vector_add(roomVector_t* v, room_t val);


void vector_free(roomVector_t* v);

