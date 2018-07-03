#include <ncurses.h>


#define FLAG_PERM  0x1
#define FLAG_TRANS 0x2
#define FLAG_DESTR 0x4

typedef struct tile{
	chtype symbol;
  int flags;
} tile_t;

typedef struct map{
	tile_t * buffer;
	int height;
 	int width;
} map_t;	


#define UNPACK(varname, map_ptr) tile_t (*varname)[(map_ptr)->width] = (tile_t (*)[(map_ptr)->width]) map_ptr->buffer 

/**
 * This macros defines a new variable which allows to access map_t.buffer
 * as a two-dimensional array with width of map_t.width.
 * 
 * Example:
 * void map_foo(map_t* map_, int x, int y) {
 * 		UNPACK(map, map_);
 * 		if(map[x][y] == '@') {
 * 			...
 * 		}
 * }
*/
#define UNPACK(varname, map_ptr) char (*varname)[(map_ptr)->width] = (char (*)[(map_ptr)->width]) map_ptr->buffer 

