#include "actor.h"
#include "behave.h"
#include "common.h"
#include "roomvector.h"
#include <stdlib.h>

actors_vt* create_new_vector(int num) {
	actors_vt* new_Vect = (actors_vt*)malloc(sizeof(actors_vt));
	new_Vect->data = (actor_t**)calloc(num,
	                                        sizeof(actor_t*));
	new_Vect->length = 0;
	new_Vect->capacity = num;
	return new_Vect;
}

actor_t * make_player() {
	actor_t* player = calloc(1, sizeof(actor_t));
	player->symbol = '@' | COLOR_PAIR(2);
	player->flags |= FLAG_ISPLAYER;
	player->behave = behave_player;
  player->inventory = calloc(1, sizeof(inventory_t));
  player->inventory->data = calloc(10, sizeof(item_t));
  player->inventory->capacity = 10;
  return player;
}

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
	return actors;
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

void free_actor(actor_t *); //FIXME: Write me!!!

void free_actors(actors_vt* vect, bool is_Full) {
	// FIXME: Who will kill them?
	if(is_Full)
		for(int i = 0; i < vect -> length; i++)
			free(vect->data[i]);
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
