#include "tree.h"
#include "vector_tree.h"
#include "common.h"
#pragma once
typedef struct room {
	int x1, y1, x2, y2;
	int S;
	int lenX, lenY;
	int doorX[2], doorY[2];
} room_t;
void room_free(room_t* room);
void createLine (int x1, int y1, int x2, int y2, char ch, int COLOR, map_t* map);
void divRoom (tree_t* parent, vector_t* leaves, map_t* map);
void div_walls_of_rooms(tree_t* leaves, map_t* map);
