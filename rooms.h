#pragma once
#include "common.h"
typedef struct room_tree {
	int x1, y1, x2, y2;
	int S;
	int len_x, len_y;
	int door_x[2], door_y[2];
} room_tree_t;
typedef struct vector_room {
	struct tree_room_t ** data;
	size_t len;
	size_t max_len;
} vector_room_t;
typedef struct tree_room_t {
	struct tree_room_t *left, *right, *parent;
	struct room_tree * value;
} tree_room_t;
tree_room_t* tree_create (void* value);
void tree_add (tree_room_t* tree, void* value);
void tree_destroy(tree_room_t* tree);
tree_room_t* add_node(tree_room_t* tree, room_tree_t* value);
void tree_dfs(tree_room_t* tree, void(*callback)(void*));
vector_room_t* vector_room_init (int len);
void vector_room_add (vector_room_t* v, tree_room_t* value);
tree_room_t* vector_room_get (vector_room_t* v, size_t i);
void vector_room_free(vector_room_t* v);
vector_room_t* vector_room_copy (vector_room_t* v);
int vector_get_len (vector_room_t* v);
void room_free(room_tree_t* room);
void create_line (int x1, int y1, int x2, int y2, char ch, int COLOR, map_t* map);
void div_room (tree_room_t* parent, vector_room_t* leaves);
void div_walls_of_rooms(tree_room_t* leaves, map_t* map);
