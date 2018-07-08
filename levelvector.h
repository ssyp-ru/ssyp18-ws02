#pragma once

#include "common.h"
#include <ncurses.h>

levels_vt* lvector_init(int L);

level_t* lvector_get(levels_vt* v, int i);

void lvector_add(levels_vt* v, level_t* val);

void lvector_free(levels_vt* v);
