#pragma once

#include "common.h"
#include <ncurses.h>

level_vector_t* lvector_init(int L);

level_t* lvector_get(level_vector_t* v, int i);

void lvector_add(level_vector_t* v, level_t* val);

void lvector_free(level_vector_t* v);
