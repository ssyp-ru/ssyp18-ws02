#include "game.h"
#include "common.h"
#include "level.h"
#include "actor.h"
#include "roomvector.h"
#include <ncurses.h>

actor_t * make_player() {
	actor_t* player = calloc(1, sizeof(actor_t));
	player->symbol = '@' | COLOR_PAIR(2);
	player->id = 1;
	player->flags |= FLAG_ISPLAYER;
	player->behave = behave_player;
  player->inventory = calloc(1, sizeof(inventory_t));
  player->inventory->data = calloc(10, sizeof(item_t));
  player->inventory->capacity = 10;
  return player;
}

actor_t * make_monster() {
	actor_t* monster = calloc(1, sizeof(actor_t));
	monster->symbol = 'O' | COLOR_PAIR(2);
	monster->id = 2;
	monster->flags |= FLAG_CANWALK;
	monster->behave = behave_monster;
	monster->view_radius = 4;
  monster->inventory = calloc(1, sizeof(inventory_t));
  monster->inventory->data = calloc(10, sizeof(item_t));
  monster->inventory->capacity = 10;
	monster->state = 0;
  return monster;
}

actors_vt* init_actors(level_t* level,
                     int amount_of_entities) {
	actors_vt* actors = create_new_vector(amount_of_entities + 1);
  actor_t * player = make_player();
	player->level = level;
	int room_number;
	if ( level->map->rooms) {
		room_number = rand() % level->map->rooms->length;

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
		//int room_number = rand() % level->map->rooms->length;
		room_number++;
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

void init_colors(){
start_color();
init_color(COLOR_MAGENTA, 300, 300,300);
init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
init_pair(2, COLOR_RED, COLOR_BLACK);
init_pair(3, COLOR_BLACK, COLOR_BLUE);
}

int main() {
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, true);
	init_GUI();
	init_colors();
	start_game();
	endwin();
}
