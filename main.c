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

	map_t* map = malloc(sizeof(map_t));
	map->buffer = calloc(width * height, sizeof(int));
	map->width = width;
	map->height = height;

	UNPACK(mapArray, map);

	initscr();

	time_t t;
	srand((unsigned) time(&t));

	for(int i = 0 ; i < width; i++)
		for(int j = 0; j < height; j++)
			mapArray[j][i] = '.';

	buildWall(map, width, height, true, 0, 0);

	drawMap(map);

	getch();

	endwin();

	return 0;
}
