#pragma once

#include "common.h"
#include <ncurses.h>

rooms_vt* vector_init(int L);

room_t vector_get(rooms_vt* v, int i);

void vector_add(rooms_vt* v, room_t val);

void vector_free(rooms_vt* v);
