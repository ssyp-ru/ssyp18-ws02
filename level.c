#include "level.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "actor.h"
#include "genmap.h"
level_t* init_level(int width,
                    int height) {
	level_t * level = calloc(1, sizeof(level_t));

	map_t * _map = create_map(height, width);
	/*_map = mapgen_shrew(_map);
	_map = mapgen_rooms_shrew(_map);
	_map = make_walls_shrew(_map);*/
	_map = mapgen_recdev(_map);
	level->map = _map;
	int num = 200;
	gen_feature(level, num);

	/*extern features_vt * prototypes;

void free_protofeatures();

void init_protofeatures();
	
void gen_feature(level_t * level, int num);

void gen_stair(levels_vt * levels, int num);
*/

	return level;
}

void free_level(level_t* level) {
	free_map(level->map);
  kd_delete(level->features);
	free(level);
}
