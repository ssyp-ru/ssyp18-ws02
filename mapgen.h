#pragma once

#define FLAG_SOLID 0x1
#define FLAG_TRANS 0x2
#define FLAG_DESTR 0x4

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include "common.h"
#include "roomvector.h"

#define UNPACK(varname, map_ptr)       \
  tile_t(*varname)[(map_ptr)->width] = \
      (tile_t(*)[(map_ptr)->width])map_ptr->buffer

void draw_map(map_t* map);

map_t* generate_map(int width, int height);

void free_map(map_t* map);
