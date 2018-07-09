#include "common.h"
#include "levelvector.h"
#include <stdlib.h>
#include "level.h"

level_t* lvector_get(levels_vt* v, int i) {
	return v->data[i];
}

levels_vt* lvector_init(int L) {
	levels_vt* v = calloc(1, sizeof(levels_vt));// (levels_vt*)malloc(sizeof(
	                //        levels_vt));

	v->data = (level_t**) calloc(L, sizeof(level_t*));
	v->length = 0;
	v->capacity = L;

	return v;
}

void lvector_add(levels_vt* v, level_t* val) {
	v->data[v->length] = val;

	if (++v->length == v->capacity) {
		v->capacity *= 2;
		v->data = realloc(v->data, v->capacity * sizeof(level_t));
	}
}

void lvector_free(levels_vt* v) {
	// We should destroy all maps here.
	for(int i = 0; i < v->length; i++) {
		free_level(lvector_get(v, i));
	}
	free(v->data);
	free(v);
}

