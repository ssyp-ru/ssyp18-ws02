#include "rooms.h"
#include <string.h>
#include <stdlib.h>
vector_room_t* vector_room_init (int len) {
	vector_room_t* vector = malloc(sizeof(vector_room_t));
	vector->len = 0;
	if(len < 16)
		len = 16;
	vector->max_len = len;
	vector->data = calloc(len, sizeof(vector_room_t*));
	return vector;
}
void vector_room_add (vector_room_t* v, tree_room_t* value) {
	if(v->len >= v->max_len) {
		v->max_len = v->len * 2;
		room_tree_t** data_help = realloc(v->data, sizeof(room_tree_t*) * v->max_len);
		if (data_help != NULL) {
			v->data = data_help;
		} else {
			fprintf(stderr, "realloc could not allocate memory");
			exit(1);
			return;
		}
	}
	v->data[v->len++] = value;
}
tree_room_t* vector_room_get (vector_room_t* v, size_t i) {
	return v->data[i];
}
void vector_room_free(vector_room_t* v) {
	free(v->data);
	free(v);
}
vector_room_t* vector_room_copy (vector_room_t* v) {
	vector_room_t* vector = vector_room_init(v->max_len);
	vector->len = vector->max_len;
	memcpy(vector->data, v->data, v->len*sizeof(void*));
	return vector;
}
int vector_get_len (vector_room_t* v) {
	return v->len;
}
