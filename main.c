#include "level.h"

int main() {
	int width = 60;
	int height = 60;

	int amountOfEntities = 0;

	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, true);

	level_t* level = initLevel(amountOfEntities, width, height);

	while (updateLevel(level))
		;

	freeLevel(level);

	endwin();
}
