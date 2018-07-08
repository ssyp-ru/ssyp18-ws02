#pragma once

#include <ncurses.h>
#include "common.h"


roomVector_t* vector_init(int L);


room_t vector_get(roomVector_t* v, int i);


void vector_add(roomVector_t* v, room_t val);


void vector_free(roomVector_t* v);

