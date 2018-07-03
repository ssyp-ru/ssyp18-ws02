#include "mapgen.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "common.h"


int main()
{
	int width = 60;
	int height = 60;

	initscr();
	
	start_color();


	map_t map = generateMap(width, height);
	drawMap(map);

	getch();

	endwin();

	return 0;
}
