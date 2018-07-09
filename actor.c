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
/*
actor_t * make_player() {
	actor_t* player = calloc(1, sizeof(actor_t));
	player->symbol = '@' | COLOR_PAIR(2);
	player->flags |= FLAG_ISPLAYER;
	player->behave = behave_player;
	player->view_radius = 4;
  player->inventory = calloc(1, sizeof(inventory_t));
  player->inventory->data = calloc(10, sizeof(item_t));
  player->inventory->capacity = 10;
 	player->name = "Player";
 	return player;
}

<<<<<<< HEAD
actor_t * make_monster() {
	actor_t* monster = calloc(1, sizeof(actor_t));
	monster->symbol = 'O' | COLOR_PAIR(2);
	monster->flags |= FLAG_CANWALK;
	monster->behave = behave_monster;
  monster->inventory = calloc(1, sizeof(inventory_t));
  monster->inventory->data = calloc(10, sizeof(item_t));
  monster->inventory->capacity = 10;
	monster->state = 0;
  return monster;
}
*/
/*
//FIXME: Should it realy be here?
=======
>>>>>>> 74e7e48a716724d2230318568af908f18920191e
actors_vt* init_actors(level_t* level,
                     int amount_of_entities) {
	actors_vt* actors = create_new_vector(amount_of_entities + 1);
  actor_t * player = make_player();
	player->level = level;
	if ( level->map->rooms) {
		int room_number = rand() % level->map->rooms->length;

		player->x = vector_get(level->map->rooms, room_number).x +
								vector_get(level->map->rooms, room_number).width / 2;
		player->y = vector_get(level->map->rooms, room_number).y +
								vector_get(level->map->rooms, room_number).height / 2;
	} else {
		player->x = 10;
		player->y = 10;
	}
	add_vector_elem(actors, player);
	actor_t* monster = make_monster();
	monster->level = level;
	if ( level->map->rooms) {
		int room_number = rand() % level->map->rooms->length;

		monster->x = vector_get(level->map->rooms, room_number).x +
								vector_get(level->map->rooms, room_number).width / 2;
		monster->y = vector_get(level->map->rooms, room_number).y +
								vector_get(level->map->rooms, room_number).height / 2;
	} else {
		monster->x = 10;
		monster->y = 10;
	}

	add_vector_elem(actors, monster);
	level->actors = actors; 
	return actors;
}
*/
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
