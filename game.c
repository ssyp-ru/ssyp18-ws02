#include <ncurses.h>
#include <stdbool.h>
#include <string.h>
#include "common.h"
#include "levelvector.h"
#include "game.h"
#include "roomvector.h"
#include "level.h"
#include "actor.h"
#include "GUI.h"
#include "feature.h"

struct {
	actors_vt** queue;
	int it;
	int id;
} game_state;

void add_actor(actor_t* self){
	add_vector_elem(game_state.queue[game_state.it + 1], self);
}

//Why the hell do we accept queue as an argument?
void main_cycle(actors_vt * actors, actors_vt ** queue,
                size_t qlen, levels_vt* levels,
							 	feature_t * features,msgs_t * msgs) {
	bool exit = false;
	game_state.queue = queue;
	for(int it = 0; !exit; it = (it+1)%qlen) {
		render(actors->data[0], actors, msgs);
		refresh();
		game_state.it = it;
		for(int i = 0; !exit && i < queue[it]->length; i++) {
			actor_t * current = actor_get(queue[it], i);
			if (current->flags & FLAG_ACTOR_DEAD){
				int i = 0;
				for (; i < actors->length && current->id != actors->data[i]->id; i++);
				delete_actor(actors, i);
				break;
			}
			int new_index = (it + current->behave(current)) % qlen;
			if(new_index == it)
				exit = true;
			add_vector_elem(queue[new_index], current);
		}
		queue[it]->length = 0;
	}
}

void destroy_the_world(actors_vt* actors,
                       levels_vt * levels,
                       actors_vt ** queue, int qlen) {
	lvector_free(levels);
	free_actors(actors);
  for(int i = 0; i < qlen; i++) {
    free(queue[i]->data);
    free(queue[i]);
  }
	free(queue);
}

actors_vt* init_actors(level_t* level,
                     int amount_of_entities) {
	actors_vt* actors = create_actor_vector(amount_of_entities + 1);
  actor_t * player = make_player();
	player->level = level;
	if ( level->map->rooms) {
		int room_number = rand() % level->map->rooms->length;

		player->x = rooms_vector_get(level->map->rooms, room_number).x +
								rooms_vector_get(level->map->rooms, room_number).width / 2;
		player->y = rooms_vector_get(level->map->rooms, room_number).y +
								rooms_vector_get(level->map->rooms, room_number).height / 2;
	} else {
		player->x = 10;
		player->y = 10;
	}
	add_vector_elem(actors, player);
	for(int k = 0; k < amount_of_entities; k++){
		actor_t * goblin = make_goblin();
		goblin->level = level;
		goblin->level->actors = actors;
		goblin->x = rand() % level->map->width;
		goblin->y = rand() % level->map->height;
		add_vector_elem(actors, goblin);
	}
	player->level->actors = actors;
	return actors;
}

msgs_t * init_mes(){
	msgs_t * msgs = calloc(1, sizeof(msgs_t));
	msgs->max_size = 1000;
	msgs->buffer = calloc(msgs->max_size, sizeof(msg_t));
	msgs->size = 1;
	msgs->cur = 0;
	msgs->buffer[0].line = "There is nothing here!";
	return msgs;
}

void send_mes(msgs_t * log, char * str) {
  size_t len = strlen(str);
  log->buffer[log->cur].line = calloc(len, sizeof(char));
  strcpy(log->buffer[log->cur].line, str);
  log->buffer[log->cur].length = len;
}


void start_game() {
	time_t t;
	srand((unsigned)time(&t));
	msgs_t * msgs = init_mes();
	levels_vt* levels = lvector_init(1);
	lvector_add(levels, init_level(30, 30));
	actors_vt* actors = init_actors(lvector_get(levels, 0), 1);
	actors_vt ** queue = calloc(100, sizeof(actors_vt*));
	for(int i = 0; i < 100; i++)
		queue[i] = create_actor_vector(1);
	for(int i = 0; i < actors->length; i++)
		add_vector_elem(queue[0], actor_get(actors, i));
	main_cycle(actors, queue, 100, levels, NULL, msgs);
	free(msgs->buffer);
	free(msgs);
	destroy_the_world(actors, levels, queue, 100);
}


