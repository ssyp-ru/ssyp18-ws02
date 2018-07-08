#pragma once

typedef struct lister_t {
	struct lister_t* next;
	void* val;
} list;

// Insert before the first element
list* list_queue(list* lis, void* val);
// Insert after the first element
list* list_insert(list* lis, void* val);
// Insert after the last element
list* list_append(list* lis, void* val);
// Insert the list after the last element
list* list_concat(list* first, list* second);

// Remove the first element
list* list_deque(list* lis);
// Removes the entire list
void list_delete(list* lis);

// Applies given callback on each member in the list.
void list_map(list* lis, void(*callback)(void*));

/**
 * Applies given callback on each member in the list.
 * Is equal to callback(get(0), callback(get(1), callback(get(2), ...)))
*/
void* list_fold(list* lis, void* init_val, void*(*callback)(void*,void*));
