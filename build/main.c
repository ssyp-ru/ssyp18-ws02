#include <ncurses.h>
#include "level.h"
int main()
{
	int amountOfEntities = 0;

	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, true);
	int width = 60;
	int height = 60;
	getmaxyx(stdscr, height, width);	

	level_t* level = initLevel(amountOfEntities, width, height);	

	while(updateLevel(level));

	freeLevel(level);

	endwin();
}
