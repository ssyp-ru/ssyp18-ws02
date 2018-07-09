#include <stdlib.h>
#include "common.h"
#include "render.h"
#include <ncurses.h>
void render (map_t* map, int scrollx, int scrolly) {
	UNPACK(field, map);
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 37; j++) {
			mvaddch(j, i, field[j +scrolly][i+scrollx].symbol);
		}
	}
}
