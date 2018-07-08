#include "datastruct.h"

#include <stdlib.h>

list* list_queue(list* lis, void* val) {
	list* ll = malloc(sizeof(list));
	ll->next = lis;
	ll->val = val;
	return ll;
}

list* list_insert(list* lis, void* val) {
	list* ll = malloc(sizeof(list));
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

list* list_concat(list* first, list* second) {
	if(second == NULL) return first;
	if(first != NULL) {
		list* tmp = first;
		while(tmp->next != NULL) tmp = tmp->next;
		tmp->next = second;
		return first;
	} else {
		return second;
	}
}

list* list_append(list* lis, void* val) {
	list* lp = lis;
	if(lp != NULL) {
		while(lp->next != NULL) lp = lp->next;
		list_insert(lp, val);
		return lis;
	} else {
		return list_insert(lp, val);
	}
}

list* list_deque(list* lis) {
	if(lis != NULL) {
		list* nl = lis->next;
		free(lis);
		return nl;
	}
	return NULL;
}

void list_delete(list* lis) {
	while(lis != NULL) {
		lis = list_deque(lis);
	}
}

void list_map(list* lis, void(*callback)(void*)) {
	while(lis != NULL) {
		callback(lis->val);
		lis = lis->next;
	}
}

void* list_fold(list* lis, void* init_val, void*(*callback)(void*,void*)) {
	if(lis == NULL) return init_val;
	return list_fold(lis->next, callback(lis->val, init_val), callback);
}