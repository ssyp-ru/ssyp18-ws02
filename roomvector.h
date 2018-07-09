#pragma once

#include "common.h"
#include <ncurses.h>

rooms_vt* rooms_vector_init(int L);

room_t rooms_vector_get(rooms_vt* v, int i);

void rooms_vector_add(rooms_vt* v, room_t val);

void rooms_vector_free(rooms_vt* v);
