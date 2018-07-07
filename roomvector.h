#pragma once

#include "common.h"
#include <ncurses.h>

room_vector_t* vector_init(int L);

room_t vector_get(room_vector_t* v, int i);

void vector_add(room_vector_t* v, room_t val);

void vector_free(room_vector_t* v);
