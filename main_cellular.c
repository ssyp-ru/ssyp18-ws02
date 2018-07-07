#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "genmap.h"
#include "common.h"

int main() {
	initscr();
	noecho();

	int size_x = 50, size_y = 50;
	int birth_limit  = 4, death_limit = 3;
	int num_of_steps = 5;
	float chance_to_be_alive = 0.4;

	map_t * map = create_map(size_y, size_x);
	map         = mapgen_cellular(map, chance_to_be_alive);
	map         = simulation_step_cellular(map, birth_limit,
	                                       death_limit, num_of_steps);
	print_map(map);
	getch();
	endwin();
	free_map(map);
	return 0;
}

