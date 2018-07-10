#pragma once

typedef struct list {
	struct list* next;
	void* val;
} list_t;

// Insert before the first element
list_t* list_queue(list_t* lis, void* val);
// Insert after the first element
list_t* list_insert(list_t* lis, void* val);
// Insert after the last element
list_t* list_append(list_t* lis, void* val);
// Insert the list_t after the last element
list_t* list_concat(list_t* first, list_t* second);

// Remove the first element
list_t* list_deque(list_t* lis);
// Removes the entire list_t
void list_delete(list_t* lis);

list_t* list_remove(list_t* list, int n);
// Applies given callback on each member in the list_t.
void list_map(list_t* lis, void(*callback)(void*));

/**
 * Applies given callback on each member in the list_t.
 * Is equal to callback(get(0), callback(get(1), callback(get(2), ...)))
*/
void* list_fold(list_t* lis, void* init_val, void*(*callback)(void*,void*));
