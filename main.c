#include "level.h"

int main() {
	int width = 60;
	int height = 60;

	int amount_of_entities = 0;

	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, true);

	level_t* level = init_level(amount_of_entities, width, height);

	while (update_level(level))
		;

	free_level(level);

	endwin();
}
