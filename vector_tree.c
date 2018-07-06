#include "rooms.h"
#include <string.h>
#include "tree.h"
#include "vector_tree.h"
#include <stdlib.h>
vector_t* vector_init (int len) {
	vector_t* vector = malloc(sizeof(vector_t));
	vector->len = 0;
	if(len < 16)
		len = 16;
	vector->maxLen = len;
	vector->data = calloc(len, sizeof(room_t));
	return vector;
}
void vector_add (vector_t* v, tree_t value) {
	if(v->len >= v->maxLen) {
		v->maxLen = v->len * 2;
		tree_t * data_help = realloc(v->data, sizeof(tree_t) * v->maxLen);
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
tree_t vector_get (vector_t* v, size_t i) {
	return v->data[i];
}
void vector_free(vector_t* v) {
	free(v->data);
	free(v);
}
vector_t* vector_copy (vector_t* v) {
	vector_t* vector = vector_init(v->maxLen);
	vector->len = vector->maxLen;
	memcpy(vector->data, v->data, v->len*sizeof(void*));
	return vector;
}
