#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "genmap.h"
#include "common.h"

int main() {
	initscr();
	noecho();
	srand(time(NULL));

	int size_x = 50, size_y = 40;

	map_t * map = create_map(size_y, size_x);
	map         = mapgen_recdev(map);
	print_map(map);
	getch();
	endwin();
	free_map(map);
	return 0;
}

