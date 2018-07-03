struct map_t {
	char * buffer;
	int height, width;
} map_t;


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
