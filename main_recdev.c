#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "genmap.h"
#include "common.h"
#include "GUI.h"
#include "feature_prototypes.h"
#include "level.h"

int main() {
	initscr();
	noecho();
	srand(time(NULL));
	int size_x = 50, size_y = 40, num = 10;
	level_t * level = init_level(size_x, size_y);
	level->map = mapgen_recdev(level->map); 
	init_protofeatures();
	
	gen_feature(level, num);
	box_t box;
	box.x = 0;
	box.y = 0;
	box.width = 50;
 	box.height = 40;
	features_vt * fvec = collect(level->features, box);
	print_map(level->map);
	getch();
	draw_features(fvec);
	getch();
	endwin();
	free_level(level);
	features_vector_destroy(fvec);
	return 0;
}

