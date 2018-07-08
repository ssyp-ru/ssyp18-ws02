#include "roomvector.h"
#include "stdlib.h"

room_t vector_get(room_vector_t* v, int i) {
	return v->data[i];
}

room_vector_t* vector_init(int L) {
	room_vector_t* v = (room_vector_t*)malloc(sizeof(
	                       room_vector_t));

	v->data = (room_t*)calloc(L, sizeof(room_t));
	v->length = 0;
	v->capacity = L;

	return v;
}

void vector_add(room_vector_t* v, room_t val) {
	v->data[v->length] = val;

	if (++v->length == v->capacity) {
		v->capacity *= 2;
		v->data = realloc(v->data, v->capacity * sizeof(room_t));
	}
}

void vector_free(room_vector_t* v) {
	free(v->data);
	free(v);
}
