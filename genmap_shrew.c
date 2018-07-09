#include <ncurses.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "genmap.h"

map_t * mapgen_shrew(map_t *_map) {
	int size_x = _map->width;
	int size_y = _map->height;
	UNPACK(map, _map);
	int origin_x, origin_y, index = 1;
	int rand4, count_cells = 0.5 * size_x * size_y;
	for(int k = 0; k < size_y; k++) {
		for(int i = 0; i < size_x; i++) {
			map[k][i].flags |= FLAG_SOLID;
		}
	}
	origin_x = rand() % (size_x - 3) + 1;
	origin_y = rand() % (size_y - 3) + 1;
	map[origin_y][origin_x].flags &= ~FLAG_SOLID;
	while(index < count_cells) {
		rand4 = rand() % 4;
		int counter = 0;
		for(int i = -1; i <= 1; i++) {
			for(int k = -1; k <= 1; k++) {
				if(k != 0 || i != 0)
					counter += !(map[origin_y + k][origin_x + i].flags &
					             FLAG_SOLID);
			}
		}
		if(map[origin_y][origin_x].flags & FLAG_SOLID
		        && counter < 4) {
			map[origin_y][origin_x].flags &= ~FLAG_SOLID;
			index++;
		} else {
			origin_x = rand() % (size_x - 3) + 1;
			origin_y = rand() % (size_y - 3) + 1;
		}
		switch(rand4) {
		case 0:
			if(origin_y - 1 > 0)
				origin_y--;
			break;
		case 1:
			if(origin_y + 2 < size_y)
				origin_y++;
			break;
		case 2:
			if(origin_x - 1 > 0)
				origin_x--;
			break;
		case 3:
			if(origin_x + 2 < size_x)
				origin_x++;
			break;
		}
	}
	return _map;
}

map_t * mapgen_test_room_shrew(map_t *_map, int y_pos,
                               int x_pos) {
	UNPACK(map, _map);
	for(int i1 = 0; i1 < 10; i1++) {
		for(int k1 = 0; k1 < 10; k1++) {
			map[i1 - 5 + y_pos][k1 - 5 + x_pos].flags &= ~FLAG_SOLID;
			map[i1 - 5 + y_pos][k1 - 5 + x_pos].symbol = ' ';
		}
	}
	return _map;
}

map_t * mapgen_rooms_shrew(map_t *_map) {
	int size_x = _map->width;
	int size_y = _map->height;
	UNPACK(map, _map);
	for(int i = 0; i < 5; i++) {
		int rand_x = rand() % (size_x - 15) + 2;
		int rand_y = rand() % (size_y - 15) + 2;
		int siz1  = rand() % 3 + 6;
		int siz2  = rand() % 3 + 6;
		int counter = 0;
		for(int k = rand_y; k <= rand_y + siz1; k++) {
			for(int l = rand_x; l <= rand_x + siz2; l++) {
				if(k == rand_y || k == rand_y + siz1
				        || l == rand_x || l == rand_x + siz2) {
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
	int size_x = _map->width;
	int size_y = _map->height;
	UNPACK(map, _map);
  for(int k = 1; k < size_y - 1; k++) {
    for(int i = 1; i < size_x - 1; i++) {
			if(map[k][i].flags & FLAG_SOLID) {
				if(k == 1 || k == size_y - 2 ||
				        i == 1 || i == size_x - 2) {
					map[k][i].symbol = '#' | COLOR_PAIR(1);
				} else {
					if(!(map[k - 1][i].flags & FLAG_SOLID) &&
					        !(map[k + 1][i].flags & FLAG_SOLID))
						map[k][i].symbol = '-' | COLOR_PAIR(1);
					else
						map[k][i].symbol = '|' | COLOR_PAIR(1);
					if  (map[k - 1][i].flags & FLAG_SOLID
					        && map[k + 1][i].flags & FLAG_SOLID
					        && map[k][i - 1].flags & FLAG_SOLID
					        && map[k][i + 1].flags & FLAG_SOLID)
						map[k][i].symbol = '+' | COLOR_PAIR(1);
				}
			} else {
				map[k][i].symbol = ' '|COLOR_PAIR(1);
      }
		}
	}
	return _map;
}
