#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include "graphics.h"
#include "generate.h"

int main(){
	map_t* mapp = createMap(40, 40);
	srand(time(NULL));
	int nummap = 0;
	initCurs(mapp);
	scanw("%d", &nummap);
	generateMap(mapp, nummap);
	drawWindow(mapp);
	generateRoad(mapp, nummap);
	drawWindow(mapp);
	exitWindow(mapp);
}
