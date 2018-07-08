#include "common.h"
#include "levelvector.h"
#include <stdlib.h>
#include "level.h"

level_t* lvector_get(level_vector_t* v, int i) {
	return v->data[i];
}

level_vector_t* lvector_init(int L) {
	level_vector_t* v = (level_vector_t*)malloc(sizeof(
	                        level_vector_t));

	v->data = (level_t**) calloc(L, sizeof(level_t*));
	v->length = 0;
	v->capacity = L;

	return v;
}

void lvector_add(level_vector_t* v, level_t* val) {
	v->data[v->length] = val;

	if (++v->length == v->capacity) {
		v->capacity *= 2;
		v->data = realloc(v->data, v->capacity * sizeof(level_t));
	}
}

void lvector_free(level_vector_t* v) {
	// We should destroy all maps here.
	for(int i = 0; i < v->length; i++) {
		free_level(lvector_get(v, i));
		free(v->data[i]);
	}
	free(v->data);
	free(v);
}
