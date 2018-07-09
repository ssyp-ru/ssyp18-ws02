#include "actor.h"
#include "behave.h"
#include "common.h"
#include "roomvector.h"
#include <stdlib.h>

actors_vt* create_new_vector(int num) {
	actors_vt* new_Vect = (actors_vt*)malloc(sizeof(actors_vt));
	new_Vect->data = (actor_t**)calloc(num, sizeof(actor_t*));
	new_Vect->length = 0;
	new_Vect->capacity = num;
	return new_Vect;
}

void draw_actors(actors_vt* actors) {
	for (int i = 0; i < actors->length; i++) {
		if (!(actors->data[i]->flags  & FLAG_DEAD)){
			mvaddch(actor_get(actors, i)->y, actor_get(actors, i)->x,
		  actor_get(actors, i)->symbol);
		}
	}
}

void resize_vector(actors_vt* vect) {
	vect->data = (actor_t**)realloc(vect->data,
	              sizeof(actor_t*)*vect->capacity * 2);
	vect->capacity = vect->capacity * 2;
}

void add_vector_elem(actors_vt* vect, actor_t* new_Actor) {
	if (vect->capacity - vect->length == 0)
		resize_vector(vect);
	vect->data[vect->length] = new_Actor;
	vect->length += 1;
}

actor_t* actor_get(actors_vt* vect, int num) {
	return vect->data[num];
}

actors_vt* free_actor(actors_vt* vect, int num) {
	free(vect->data[num]);
 	for (int i = 0; i < (vect->length - num); i++)
		vect->data[num + i] = vect->data[num + i + 1];
	vect -> length -= 1;	
	return vect;
}	//FIXME: Write me!!!

void free_actors(actors_vt* vect, bool is_Full) {
	// FIXME: Who will kill them?
	if(is_Full){
		for(int i = 0; i < vect -> length; i++)
		{
			free(vect->data[i]->inventory->data);
			free(vect->data[i]->inventory);
			free(vect->data[i]);
		}	
	free(vect->data);
	free(vect);
	}
}

bool update_actors(actors_vt* vect) {
	for (int i = 0; i < vect->length; i++) {
		actor_t* current_actor = actor_get(vect, i);
		if (current_actor->behave(current_actor) == 0) {
			return false;
		}
	}
	return true;
}
