typedef struct map_t {
	char * buffer;
	int height, width;
} map_t;

#define UNPACK(varname, map_ptr) char (*varname)[(map_ptr)->width] = (char (*)[(map_ptr)->width]) map_ptr->buffer 
