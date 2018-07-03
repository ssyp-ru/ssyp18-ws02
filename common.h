#pragma once

typedef struct tile{
	chtype symbol;
  int flags;
} tile_t;

typedef struct map{
	tile_t * buffer;
	int height, int width;
} map_t;	

#define UNPACK(varname, map_ptr) tile_t (*varname)[(map_ptr)->width] = (tile_t (*)[(map_ptr)->width]) map_ptr->buffer 
