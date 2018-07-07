#pragma once
#include <stdlib.h>
#include "tree.h"
typedef struct vector {
	tree_t ** data;
	size_t len;
	size_t max_len;
} vector_t;

vector_t* vector_init (int len);
void vector_add (vector_t* v, tree_t* value);
tree_t* vector_get (vector_t* v, size_t i);
void vector_free(vector_t* v);
vector_t* vector_copy (vector_t* v);
int vector_get_len (vector_t* v);
