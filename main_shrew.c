#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"
#include "genmap.h"


int main() {
	initscr();
	noecho();

	int size_x = 50;
	int size_y = 30;

	srand(time(NULL));
	//Creating map and unpacking it
	map_t * _map = create_map(size_y, size_x);
	//Creating main character, his vision and  an inventory
	//Generation of labirint with rooms
	_map = mapgen_shrew(_map);
	_map = mapgen_rooms_shrew(_map);
	//Introduce some walls
	_map = make_walls_shrew(_map);

	print_map(_map);
	getch();
	endwin();
	free_map(_map);
	return 0;

}
