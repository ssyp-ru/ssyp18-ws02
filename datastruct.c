#include "datastruct.h"

#include <stdlib.h>

list_t* list_queue(list_t* lis, void* val) {
	list_t* ll = malloc(sizeof(list_t));
	ll->next = lis;
	ll->val = val;
	return ll;
}

list_t* list_insert(list_t* lis, void* val) {
	list_t* ll = malloc(sizeof(list_t));
	ll->val = val;
	if(lis != NULL) {
		ll->next = lis->next;
		lis->next = ll;
		return lis;
	} else {
		ll->next = NULL;
		return ll;
	}
}

list_t* list_concat(list_t* first, list_t* second) {
	if(second == NULL) return first;
	if(first != NULL) {
		list_t* tmp = first;
		while(tmp->next != NULL) tmp = tmp->next;
		tmp->next = second;
		return first;
	} else {
		return second;
	}
}

list_t* list_append(list_t* lis, void* val) {
	list_t* lp = lis;
	if(lp != NULL) {
		while(lp->next != NULL) lp = lp->next;
		list_insert(lp, val);
		return lis;
	} else {
		return list_insert(lp, val);
	}
}

list_t* list_deque(list_t* lis) {
	if(lis != NULL) {
		list_t* nl = lis->next;
		free(lis);
		return nl;
	}
	return NULL;
}

list_t* list_remove(list_t* list, int n) {
    int i = 0;
    list_t * current = list;
    list_t * temp_node = NULL;

    if (n == 0) 
        return list_deque(list);

    for (i = 0; i < n-1; i++) {
        if (current->next == NULL) {
            return NULL;
        }
        current = current->next;
    }

    temp_node = current->next;
    current->next = temp_node->next;
    free(temp_node);

    return list;
}

void list_delete(list_t* lis) {
	while(lis != NULL) {
		lis = list_deque(lis);
	}
}

void list_map(list_t* lis, void(*callback)(void*)) {
	while(lis != NULL) {
		callback(lis->val);
		lis = lis->next;
	}
}

void* list_fold(list_t* lis, void* init_val, void*(*callback)(void*,void*)) {
	if(lis == NULL) return init_val;
	return list_fold(lis->next, callback(lis->val, init_val), callback);
}
