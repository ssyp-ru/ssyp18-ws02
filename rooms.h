#include "tree.h"
#include "vector_tree.h"
#include "common.h"
#include "dumblogger.h"
#pragma once
typedef struct room {
	int x1, y1, x2, y2;
	int S;
	int len_x, len_y;
	int door_x[2], door_y[2];
} room_t;
void room_free(room_t* room);
void create_line (int x1, int y1, int x2, int y2, char ch, int COLOR, map_t* map);
void div_room (tree_t* parent, vector_t* leaves, map_t* map);
void div_walls_of_rooms(tree_t* leaves, map_t* map);
