#include <ncurses.h>
#include <stdbool.h>
#include "common.h"
#include "levelvector.h"
#include "game.h"
#include "level.h"
#include "actor.h"
#include "GUI.h"
#include "feature.h"
#define MAP_SIZE 200


//Why the hell do we accept queue as an argument?
void main_cycle(actors_vt * actors, actors_vt ** queue,
                size_t qlen, levels_vt* levels,
							 	feature_t * features,msgs_t * msgs) {
	bool exit = false;
	for(int it = 0; !exit; it = (it+1)%qlen) {
		render(actors->data[0], actors, msgs);
		refresh();
		for(int i = 0; !exit && i < queue[it]->length; i++) {
			actor_t * current = actor_get(queue[it], i);
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
	//free_level(lvector_get(levels, 0));
	//vector_free(lvector_get(levels, 0)->map->rooms);
	free_actors(actors, true);
	for(int i = 0; i < qlen; i++)
		free_actors(queue[i], false); // Double free()? Are you crazy?
	free(queue);
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

void start_game() {
	time_t t;
	srand((unsigned)time(&t));
	feature_t * features = NULL;
	msgs_t * msgs = init_mes();
	levels_vt* levels = lvector_init(1); // Seriously? One?
	lvector_add(levels, init_level(200, 200));
	actors_vt* actors = init_actors(lvector_get(levels, 0), 10000);
	actors_vt ** queue = calloc(100, sizeof(actors_vt*));
	for(int i = 0; i < 100; i++)
		queue[i] = create_new_vector(1);
	
	for(int i = 0; i < actors->length; i++)
		add_vector_elem(queue[0], actor_get(actors, i));

	main_cycle(actors, queue, 100, levels, features, msgs);
	free(msgs->buffer);
	free(msgs);
	destroy_the_world(actors, levels, queue, 100);
}


