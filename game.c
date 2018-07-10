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
#include "kdtree.h"

#define GAME_QLEN 128
struct {
	actors_vt** queue;
	int it; 
  int qlen;
	int id;
  msgs_t * log;
} game_state;

void add_actor(actor_t* self){
	add_vector_elem(game_state.queue[(game_state.it + 1) % game_state.qlen], self);
}

void main_cycle(actors_vt * actors, 
                levels_vt* levels,
							 	msgs_t * msgs) {
	bool exit = false;
	actors_vt ** queue = game_state.queue;
	for(int it = 0; !exit; it = (it+1)%game_state.qlen) {
    game_state.it = it;
		render(actors->data[0], actors, msgs);
		refresh();
		for(int i = 0; !exit && i < queue[it]->length; i++) {
			actor_t * current = actor_get(queue[it], i);
			if (current->flags & FLAG_ACTOR_DEAD){
				int i = 0;
				for (; i < actors->length && current->id != actors->data[i]->id; i++);
				delete_actor(actors, i);
				break;
			}
      // FIXME: An abominable crutch for feature display.
			int new_index = (it + current->behave(current)) % game_state.qlen;
			if(new_index == it) {
				exit = true;
        continue;
      }
      if (current->flags & FLAG_ACTOR_ISPLAYER) {
        kdtree_t * here = kd_check(current->level->features,
                                    current->x, current->y);
        if (here) {
          put_message(here->node->description);
        } else {
          put_message("Nothing to see here!");
        }
      }
			add_vector_elem(queue[new_index], current);
		}
		queue[it]->length = 0;
	}
}

void destroy_the_world(actors_vt* actors,
                       levels_vt * levels) {
	lvector_free(levels);
	free_actors(actors);
  for(int i = 0; i < game_state.qlen; i++) {
    free(game_state.queue[i]->data);
    free(game_state.queue[i]);
  }
	free(game_state.queue);
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

void put_message(char * str) {
  size_t len = strlen(str)+1;
  game_state.log->cur++;
  game_state.log->size++;
  game_state.log->buffer[game_state.log->cur].line = calloc(len, sizeof(char));
  strcpy(game_state.log->buffer[game_state.log->cur].line, str);
  game_state.log->buffer[game_state.log->cur].length = len;
  game_state.log->cur %= game_state.log->max_size;
}

void init_messages(){
	game_state.log = calloc(1, sizeof(msgs_t));
	game_state.log->max_size = 1000;
	game_state.log->buffer = calloc(game_state.log->max_size, sizeof(msg_t));
	game_state.log->size = 1;
	game_state.log->cur = 0;
  put_message("There is nothing here!");
}

void close_messages() {
  for(int i = 0; i < game_state.log->size; i++)
    if (game_state.log->buffer[i].line)
      free(game_state.log->buffer[i].line);
  free(game_state.log->buffer);
  free(game_state.log);
}

void init_timequeue(actors_vt * actors) {
  game_state.qlen = GAME_QLEN;
	game_state.queue = calloc(game_state.qlen, sizeof(actors_vt*));
	for(int i = 0; i < game_state.qlen; i++)
		game_state.queue[i] = create_actor_vector(1);
	for(int i = 0; i < actors->length; i++)
		add_vector_elem(game_state.queue[0], actor_get(actors, i));
}

void start_game() {
	time_t t;
	srand((unsigned)time(&t));
	init_messages();
	levels_vt* levels = lvector_init(1);
	lvector_add(levels, init_level(120, 80));
	actors_vt* actors = init_actors(lvector_get(levels, 0), 1000);
  init_timequeue(actors);
  /*for(int i = 0; i < 3; i++)
  {
    actor_t * monster = make_monster();
    monster->level = lvector_get(levels, 0);
    monster->x = rand() % 120;
    monster->y = rand() % 80;
    add_actor(monster);
  }*/
	main_cycle(actors, levels, game_state.log);
  close_messages();
	destroy_the_world(actors, levels);
}


