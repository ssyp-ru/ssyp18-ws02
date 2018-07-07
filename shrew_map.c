#include <ncurses.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "genmap.h"

map_t * mapgen_shrew(map_t *_map) {
	int sizeX = _map->width;
	int sizeY = _map->height;
	UNPACK(map, _map);
	int originX, originY, index = 1;
	int rand4, countCells = 0.5 * sizeX * sizeY;
	for(int i = 0; i < sizeX; i++) {
		for(int k = 0; k < sizeY; k++) {
			map[k][i].flags |= FLAG_SOLID;
		}
	}
	originX = rand() % (sizeX - 3) + 1;
	originY = rand() % (sizeY - 3) + 1;
	map[originY][originX].flags &= ~FLAG_SOLID;
	while(index < countCells) {
		rand4 = rand() % 4;
		int counter = 0;
		for(int i = -1; i <= 1; i++) {
			for(int k = -1; k <= 1; k++) {
				if(k != 0 || i != 0)
					counter += !(map[originY + k][originX + i].flags &
					             FLAG_SOLID);
			}
		}
		if(map[originY][originX].flags & FLAG_SOLID
		        && counter < 4) {
			map[originY][originX].flags &= ~FLAG_SOLID;
			index++;
		} else {
			originX = rand() % (sizeX - 3) + 1;
			originY = rand() % (sizeY - 3) + 1;
		}
		switch(rand4) {
		case 0:
			if(originY - 1 > 0)
				originY--;
			break;
		case 1:
			if(originY + 2 < sizeY)
				originY++;
			break;
		case 2:
			if(originX - 1 > 0)
				originX--;
			break;
		case 3:
			if(originX + 2 < sizeX)
				originX++;
			break;
		}
	}
	return _map;
}

map_t * mapgen_test_room_shrew(map_t *_map, int yPos,
                               int xPos) {
	UNPACK(map, _map);
	for(int i1 = 0; i1 < 10; i1++) {
		for(int k1 = 0; k1 < 10; k1++) {
			map[i1 - 5 + yPos][k1 - 5 + xPos].flags &= ~FLAG_SOLID;
			map[i1 - 5 + yPos][k1 - 5 + xPos].symbol = ' ';
		}
	}
	return _map;
}

map_t * mapgen_rooms_shrew(map_t *_map) {
	int sizeX = _map->width;
	int sizeY = _map->height;
	UNPACK(map, _map);
	for(int i = 0; i < 5; i++) {
		int randX = rand() % (sizeX - 15) + 2;
		int randY = rand() % (sizeY - 15) + 2;
		int siz1  = rand() % 3 + 6;
		int siz2  = rand() % 3 + 6;
		int counter = 0;
		for(int k = randY; k <= randY + siz1; k++) {
			for(int l = randX; l <= randX + siz2; l++) {
				if(k == randY || k == randY + siz1
				        || l == randX || l == randX + siz2) {
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
	int sizeX = _map->width;
	int sizeY = _map->height;
	UNPACK(map, _map);
	for(int i = 0; i < sizeX; i++) {
		for(int k = 0; k < sizeY; k++) {
			if(map[k][i].flags & FLAG_SOLID) {
				if(k == 0 || k == sizeY - 1 ||
				        i == 0 || i == sizeX - 1) {
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
						map[k][i].symbol = '#' | COLOR_PAIR(6);
				}
			} else
				map[k][i].symbol = ' ';
		}
	}
	return _map;
}
