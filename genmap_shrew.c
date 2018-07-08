#include <ncurses.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "genmap.h"

map_t * mapgen_shrew(map_t *_map) {
	int size_X = _map->width;
	int size_Y = _map->height;
	UNPACK(map, _map);
	int origin_X, origin_Y, index = 1;
	int rand4, count_Cells = 0.5 * size_X * size_Y;
	for(int k = 0; k < size_Y; k++) {
		for(int i = 0; i < size_X; i++) {
			map[k][i].flags |= FLAG_SOLID;
		}
	}
	origin_X = rand() % (size_X - 3) + 1;
	origin_Y = rand() % (size_Y - 3) + 1;
	map[origin_Y][origin_X].flags &= ~FLAG_SOLID;
	while(index < count_Cells) {
		rand4 = rand() % 4;
		int counter = 0;
		for(int i = -1; i <= 1; i++) {
			for(int k = -1; k <= 1; k++) {
				if(k != 0 || i != 0)
					counter += !(map[origin_Y + k][origin_X + i].flags &
					             FLAG_SOLID);
			}
		}
		if(map[origin_Y][origin_X].flags & FLAG_SOLID
		        && counter < 4) {
			map[origin_Y][origin_X].flags &= ~FLAG_SOLID;
			index++;
		} else {
			origin_X = rand() % (size_X - 3) + 1;
			origin_Y = rand() % (size_Y - 3) + 1;
		}
		switch(rand4) {
		case 0:
			if(origin_Y - 1 > 0)
				origin_Y--;
			break;
		case 1:
			if(origin_Y + 2 < size_Y)
				origin_Y++;
			break;
		case 2:
			if(origin_X - 1 > 0)
				origin_X--;
			break;
		case 3:
			if(origin_X + 2 < size_X)
				origin_X++;
			break;
		}
	}
	return _map;
}

map_t * mapgen_test_room_shrew(map_t *_map, int y_Pos,
                               int x_Pos) {
	UNPACK(map, _map);
	for(int i1 = 0; i1 < 10; i1++) {
		for(int k1 = 0; k1 < 10; k1++) {
			map[i1 - 5 + y_Pos][k1 - 5 + x_Pos].flags &= ~FLAG_SOLID;
			map[i1 - 5 + y_Pos][k1 - 5 + x_Pos].symbol = ' ';
		}
	}
	return _map;
}

map_t * mapgen_rooms_shrew(map_t *_map) {
	int size_X = _map->width;
	int size_Y = _map->height;
	UNPACK(map, _map);
	for(int i = 0; i < 5; i++) {
		int rand_X = rand() % (size_X - 15) + 2;
		int rand_Y = rand() % (size_Y - 15) + 2;
		int siz1  = rand() % 3 + 6;
		int siz2  = rand() % 3 + 6;
		int counter = 0;
		for(int k = rand_Y; k <= rand_Y + siz1; k++) {
			for(int l = rand_X; l <= rand_X + siz2; l++) {
				if(k == rand_Y || k == rand_Y + siz1
				        || l == rand_X || l == rand_X + siz2) {
					if(!(map[k - 1][l].flags & FLAG_SOLID) &&
					        !(map[k + 1][l].flags & FLAG_SOLID) && counter < 3) {
						//Place for a door
						map[k][l].flags &= ~FLAG_SOLID;
						counter++;
					} else if(!(map[k][l - 1].flags & FLAG_SOLID) &&
					          !(map[k][l + 1].flags & FLAG_SOLID) && counter < 3) {
						//Place for a door
						map[k][l].flags &= ~FLAG_SOLID;
						counter++;
					} else
						map[k][l].flags |= FLAG_SOLID;
				} else map[k][l].flags &= ~FLAG_SOLID;
			}
		}
		counter = 0;
	}
	return _map;
}

map_t * make_walls_shrew(map_t * _map) {
	int size_X = _map->width;
	int size_Y = _map->height;
	UNPACK(map, _map);
	for(int i = 1; i < size_X - 1; i++) {
		for(int k = 1; k < size_Y - 1; k++) {
			if(map[k][i].flags & FLAG_SOLID) {
				if(k == 1 || k == size_Y - 2 ||
				        i == 1 || i == size_X - 2) {
					map[k][i].symbol = '#' | COLOR_PAIR(1);
				} else {
					if(!(map[k - 1][i].flags & FLAG_SOLID) &&
					        !(map[k + 1][i].flags & FLAG_SOLID))
						map[k][i].symbol = '-' | COLOR_PAIR(6);
					else
						map[k][i].symbol = '|' | COLOR_PAIR(6);
					if  (map[k - 1][i].flags & FLAG_SOLID
					        && map[k + 1][i].flags & FLAG_SOLID
					        && map[k][i - 1].flags & FLAG_SOLID
					        && map[k][i + 1].flags & FLAG_SOLID)
						map[k][i].symbol = '+' | COLOR_PAIR(6);
				}
			} else
				map[k][i].symbol = ' ';
		}
	}
	return _map;
}
