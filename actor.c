#include "actor.h"
#include "behave.h"
#include "common.h"

avect_t* create_new_vector(int num) {
	avect_t* newVect = (avect_t*)malloc(sizeof(avect_t));
	newVect->all_actors = (actor_t*)calloc(num, sizeof(actor_t));
	newVect->len = 0;
	newVect->capacity = num;
	return newVect;
}

avect_t* init_actors(level_t* level, int amount_of_entities) {
	avect_t* actors = create_new_vector(amount_of_entities + 1);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	int room_number = rand() % level->map->rooms->length;
	actor_t player;
	player.symbol = '@' | COLOR_PAIR(2);
	player.x = vector_get(level->map->rooms, room_number).x +
	           vector_get(level->map->rooms, room_number).width / 2;
	player.y = vector_get(level->map->rooms, room_number).y +
	           vector_get(level->map->rooms, room_number).height / 2;
	player.flags = 0;
	player.flags |= FLAG_ISPLAYER;
	player.behave = behave_player;
	player.level = level;
	add_vector_elem(actors, player);
	room_number = rand() % level->map->rooms->length;
	actor_t monster;
	monster.symbol = 'O' | COLOR_PAIR(2);
	monster.x = vector_get(level->map->rooms, room_number).x +
	           vector_get(level->map->rooms, room_number).width / 2;
	monster.y = vector_get(level->map->rooms, room_number).y +
	           vector_get(level->map->rooms, room_number).height / 2;
	monster.flags = 0;
	monster.flags |= FLAG_CANWALK;
	monster.behave = behave_monster;
	monster.level = level;
	add_vector_elem(actors, monster);
	return actors;
}

void draw_actors(avect_t* actors) {
	for (int i = 0; i < actors->len; i++) {
		if (!(actors->all_actors[i].flags  & FLAG_DEAD)){
			mvaddch(actor_get(actors, i)->y, actor_get(actors, i)->x,
		  actor_get(actors, i)->symbol);
		}
	}
}

void resize_vector(avect_t* vect) {
	vect->all_actors = (actor_t*)realloc(vect->all_actors,
	                                    sizeof(actor_t) * vect->capacity * 2);
	vect->capacity = vect->capacity * 2;
}

void add_vector_elem(avect_t* vect, actor_t newActor) {
	if (vect->capacity - vect->len == 0) resize_vector(vect);
	vect->all_actors[vect->len] = newActor;
	vect->len += 1;
}

actor_t* actor_get(avect_t* vect, int num) {
	return &vect->all_actors[num];
}

void free_actors(avect_t* vect) {
	free(vect->all_actors);
	free(vect);
}

bool update_actors(avect_t* vect) {
	for (int i = 0; i < vect->len; i++) {
		actor_t* current_actor = actor_get(vect, i);
		if (current_actor->behave(current_actor) == 0) {
			return false;
		}
	}
	return true;
}
