#include <stdlib.h>
typedef struct vector_room {
	struct tree ** data;
	size_t len;
	size_t max_len;
} vector_room_t;

vector_room_t* vector_room_init (int len);
void vector_room_add (vector_room_t* v, treeYA_t* value);
room_tree_t* vector_room_get (vector_room_t* v, size_t i);
void vector_room_free(vector_room_t* v);
vector_room_t* vector_room_copy (vector_room_t* v);
int vector_get_len (vector_room_t* v);
