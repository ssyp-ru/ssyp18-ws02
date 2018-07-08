#ifndef _GRAPHICS_03_07_18_21_55
#define _GRAPHICS_03_07_18_21_55

typedef struct cell{
	chtype symbol;
  int flags;
} cell_t;

typedef struct map{
	cell_t* cells;
	int height;	//x
 	int width;	//y
} map_t;	

#define UNPACK(varname, map_ptr) cell_t (*varname)[(map_ptr)->width] = (cell_t (*)[(map_ptr)->width]) map_ptr->cells 

void initCurs(map_t* mapp);

void rectangle(map_t* mapp, int x, int x1, int y, int y1);
  
void drawSimpleLine(map_t* mapp, int x, int x1, int y, int y1);

void drawWindow(map_t* mapp);

void exitWindow(map_t* mapp);
#endif
