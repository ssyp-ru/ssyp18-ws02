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

void main_cycle(avect_t * actors, avect_t ** queue,
                size_t qlen, level_vector_t* levels,
							 	box_t box, feature_t * features,msgs_t * msgs) {
	bool exit = false;
	for(int it = 0; !exit; it = (it+1)%qlen) {
		//draw_level(lvector_get(levels, 0), box, actors, features, msgs);	
		render(lvector_get(levels,0)->map,actors,	features, msgs, box);

		refresh();
		for(int i = 0; !exit && i < queue[it]->len; i++) {
			actor_t * current = actor_get(queue[it], i);
			int new_index = (it + current->behave(current)) % qlen;
			if(new_index == it)
				exit = true;
			add_vector_elem(queue[new_index], current);
		}
		queue[it]->len = 0;
	}
}

void destroy_the_world(avect_t* actors,
                       level_vector_t * levels,
                       avect_t ** queue, int qlen) {
	lvector_free(levels);
	free_actors(actors, true);
	for(int i = 0; i < qlen; i++)
		free_actors(queue[i], false);
	free(queue);
}

void start_game() {
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	time_t t;
	srand((unsigned)time(&t));
	feature_t * features;
	msgs_t * msgs = calloc(1, sizeof(msgs_t));
	msgs->max_size = 1000;
	msgs->buffer = calloc(msgs->max_size, sizeof(msg_t));
	msgs->size = 1;
	msgs->cur = 0;
	msgs->buffer[0].line = "There is nothing here!";
	box_t box;
	box.x = 0;
	box.y = 0;
	box.width = 70;
	box.height = 37;	
	init_GUI(box);
	level_vector_t* levels = lvector_init(1);
	lvector_add(levels, init_level(200, 200));
	avect_t* actors = init_actors(lvector_get(levels, 0), 0);
	avect_t ** queue = calloc(100, sizeof(avect_t*));
	for(int i = 0; i < 100; i++)
		queue[i] = create_new_vector(1);
	
	for(int i = 0; i < actors->len; i++)
		add_vector_elem(queue[0], actor_get(actors, i));

	main_cycle(actors, queue, 100, levels, box, features, msgs);
	destroy_the_world(actors, levels, queue, 100);
}


