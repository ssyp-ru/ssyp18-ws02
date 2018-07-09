#include "roomvector.h"
#include "stdlib.h"

room_t vector_get(rooms_vt* v, int i) {
	return v->data[i];
}

rooms_vt* vector_init(int L) {
//	rooms_vt* v = (rooms_vt*)malloc(sizeof(
	//                       rooms_vt));
	rooms_vt * v = calloc(1, sizeof(rooms_vt));
	v->data = (room_t*)calloc(L, sizeof(room_t));
	v->length = 0;
	v->capacity = L;

	return v;
}

void vector_add(rooms_vt* v, room_t val) {
	v->data[v->length] = val;

	if (++v->length == v->capacity) {
		v->capacity *= 2;
		v->data = realloc(v->data, v->capacity * sizeof(room_t));
	}
}

void vector_free(rooms_vt* v) {
	free(v->data);
	free(v);
}
