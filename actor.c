#include "actor.h"
#include "behave.h"
#include "common.h"
#include "roomvector.h"
#include <stdlib.h>

unsigned int last_actor_id  = 0;

unsigned int give_actor_id() {
  return last_actor_id++;
}

actors_vt* create_actor_vector(int num) {
	actors_vt* new_Vect = (actors_vt*)malloc(sizeof(actors_vt));
	new_Vect->data = (actor_t**)calloc(num, sizeof(actor_t*));
	new_Vect->length = 0;
	new_Vect->capacity = num;
	return new_Vect;
}

actor_t * make_player() {
	actor_t * player = calloc(1, sizeof(actor_t));
	player->symbol = '@' | COLOR_PAIR(2);
	//player->flags |= FLAG_ISPLAYER;
	player->behave = behave_player;
	player->view_radius = 4;
  player->inventory = calloc(1, sizeof(inventory_t));
  player->inventory->data = calloc(10, sizeof(item_t));
  player->inventory->capacity = 10;
 	player->name = "Player";
  player->id = give_actor_id();
 	return player;
}

actor_t * make_goblin(){
	actor_t * goblin = calloc(1, sizeof(actor_t));
	goblin->symbol = '9' | COLOR_PAIR(4);
	goblin->behave = behave_goblin;
	goblin->view_radius = 10;
	goblin->inventory = calloc(1, sizeof(inventory_t));
	goblin->inventory->data = calloc(3, sizeof(item_t));
	goblin->inventory->capacity = 3;
	goblin->name = "Goblin";
  goblin->id = give_actor_id();
	return goblin;
}

actor_t * make_monster() {
	actor_t* monster = calloc(1, sizeof(actor_t));
	monster->symbol = 'O' | COLOR_PAIR(2);
	monster->id = give_actor_id();
	monster->flags |= FLAG_ACTOR_CANWALK;
	monster->behave = behave_monster;
	monster->view_radius = 4;
  monster->inventory = calloc(1, sizeof(inventory_t));
  monster->inventory->data = calloc(10, sizeof(item_t));
  monster->inventory->capacity = 10;
	monster->state = 0;
  monster->memory = NULL;
  return monster;
}

void resize_vector(actors_vt* vect) {
	vect->data = (actor_t**)realloc(vect->data,
	              sizeof(actor_t*)*vect->capacity * 2);
	vect->capacity = vect->capacity * 2;
}

void add_vector_elem(actors_vt* vect, actor_t* new_actor) {
	if (vect->capacity - vect->length == 0)
		resize_vector(vect);
	vect->data[vect->length] = new_actor;
	vect->length += 1;
}

actor_t* actor_get(actors_vt* vect, int num) {
	return vect->data[num];
}

void delete_actor (actors_vt* vect, int num) {
  free_actor(vect->data[num]);
 	for (int i = num; i < (vect->length - num); i++)
		vect->data[i] = vect->data[i + 1];
	vect->length--;	
}	

void free_actor(actor_t * actor) {
  if (!actor)
    return;
  //if(actor->name)
  //  free(actor->name);
  if(actor->memory)
    free(actor->memory);
  if (actor->inventory) {
    if (actor->inventory->data)
      free(actor->inventory->data);
    free(actor->inventory);
  }
  free(actor);
}

void free_actors(actors_vt* vect) {
  for(int i = 0; i < vect -> length; i++)
  {
    free_actor(vect->data[i]);
  }	
	free(vect->data);
	free(vect);
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
