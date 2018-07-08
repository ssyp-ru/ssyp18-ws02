#include "roomvector.h"
#include "stdlib.h"


room_t vector_get(roomVector_t* v, int i)
{
	return v->data[i];
}


roomVector_t* vector_init(int L)
{
	roomVector_t* v = (roomVector_t*) malloc(sizeof(roomVector_t));

	v->data = (room_t*) calloc(L, sizeof(room_t));
	v->length = 0;
	v->capacity = L;

	return v;
}


void vector_add(roomVector_t* v, room_t val)
{
	v->data[v->length] = val;

	if(++v->length == v->capacity)
	{
		v->capacity *= 2;
		v->data = realloc(v->data, v->capacity * sizeof(room_t));
	}
}


void vector_free(roomVector_t* v)
{
	free(v->data);
	free(v);
}

