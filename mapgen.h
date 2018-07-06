#pragma once


#define FLAG_SOLID  0x1
#define FLAG_TRANS 0x2
#define FLAG_DESTR 0x4


#include <ncurses.h>
#include <stdlib.h>
#include <stdbool.h>
#include "roomvector.h"
#include "common.h"

#define UNPACK(varname, map_ptr) tile_t (*varname)[(map_ptr)->width] = (tile_t (*)[(map_ptr)->width]) map_ptr->buffer 





void drawMap(map_t* map);


map_t* generateMap(int width, int height);


void freeMap(map_t* map);
