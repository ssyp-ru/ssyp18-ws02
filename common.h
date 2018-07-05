#pragma once

#include <ncurses.h>


#define FLAG_SOLID 0x1
#define FLAG_TRANS 0x2
#define FLAG_DESTR 0x4

typedef struct tile{
	chtype symbol;
  int flags;
} tile_t;

typedef struct map{
	tile_t * buffer;
	int height, width;
} map_t;	


#define UNPACK(varname, map_ptr) tile_t (*varname)[(map_ptr)->width] = (tile_t (*)[(map_ptr)->width]) map_ptr->buffer 
