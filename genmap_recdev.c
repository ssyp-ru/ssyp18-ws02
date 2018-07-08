#include "genmap.h"
#include <stdlib.h>
#include <time.h>

void generate_room_recdev(map_t* map_packed, int width,
                          int height, int x, int y) {
	UNPACK(map, map_packed);

	for (int i = x; i < x + width; i++) {
		for (int j = y; j < y + height; j++) {
			map[j][i].symbol = ' ' | COLOR_PAIR(1);
			map[j][i].flags &= ~FLAG_SOLID;
		}
	}

	room_t new_room;
	new_room.x = x;
	new_room.y = y;
	new_room.width = width;
	new_room.height = height;

	vector_add(map_packed->rooms, new_room);
}

void build_wall_recdev(map_t* map_packed,
                       int width,
                       int height,
                       bool is_horizontal,
                       int x,
                       int y) {
	UNPACK(map, map_packed);

	if (is_horizontal) {
		int wall_Loc;

		if (height == 3)
			wall_Loc = y + 1;
		else
			wall_Loc = rand() % (height - 2) + 1 + y;

		for (int i = x; i < x + width; i++) {
			map[wall_Loc][i].symbol = '#' | COLOR_PAIR(1);
			map[wall_Loc][i].flags |= FLAG_SOLID;
		}

		if (width > 2 && wall_Loc - y > 2) {
			if (rand() % 10 < 9
			        || width > map_packed->width / 4 ||
			        wall_Loc - y > map_packed->height / 4) {
				build_wall_recdev(map_packed, width, wall_Loc - y,
				                  wall_Loc - y >= width, x, y);
			} else if (width > map_packed->width / 20 ||
			           wall_Loc - y > map_packed->height / 20)
				generate_room_recdev(map_packed, width, wall_Loc - y, x, y);
		} else if (width >= 2 && wall_Loc - y >= 2)
			generate_room_recdev(map_packed, width, wall_Loc - y, x, y);

		if (width > 2 && height - wall_Loc + y - 1 > 2) {
			if (rand() % 10 < 9
			        || width > map_packed->width / 4 ||
			        height - wall_Loc + y - 1 > map_packed->height / 4)
				build_wall_recdev(map_packed, width,
				                  height - wall_Loc + y - 1,
				                  height - wall_Loc + y - 1 >= width, x,
				                  wall_Loc + 1);
			else if (width > map_packed->width / 20 ||
			         height - wall_Loc + y - 1 > map_packed->height / 20)
				generate_room_recdev(map_packed, width,
				                     height - wall_Loc + y - 1, x,
				                     wall_Loc + 1);
		} else if (width >= 2
		           && height - wall_Loc + y - 1 >= 2)
			generate_room_recdev(map_packed, width,
			                     height - wall_Loc + y - 1, x, wall_Loc + 1);

		int gate_Loc;
		int it = 0;
		int possible_Hole_Spots[width];

		for (int i = x; i < x + width; i++)
			if ((map[wall_Loc - 1][i].flags & FLAG_SOLID) == 0
			        &&
			        (map[wall_Loc + 1][i].flags & FLAG_SOLID) == 0) {
				possible_Hole_Spots[it] = i;
				it++;
			}

		if (it == 1)
			gate_Loc = possible_Hole_Spots[0];
		else
			gate_Loc = possible_Hole_Spots[rand() % it];

		map[wall_Loc][gate_Loc].symbol = '.' | COLOR_PAIR(
		                                   1);
		map[wall_Loc][gate_Loc].flags &= ~FLAG_SOLID;
	} else {
		int wall_Loc;

		if (width == 3)
			wall_Loc = x + 1;
		else
			wall_Loc = rand() % (width - 2) + 1 + x;

		for (int i = y; i < y + height; i++) {
			map[i][wall_Loc].symbol = '#' | COLOR_PAIR(1);
			map[i][wall_Loc].flags ^= FLAG_SOLID;
		}
		if (wall_Loc - x > 2 && height > 2) {
			if (rand() % 10 < 9
			        || wall_Loc - x > map_packed->width / 4 ||
			        height > map_packed->height / 4)
				build_wall_recdev(map_packed, wall_Loc - x, height,
				                  height >= wall_Loc - x, x, y);
			else if (wall_Loc - x > map_packed->width / 20 ||
			         height > map_packed->height / 20)
				generate_room_recdev(map_packed, wall_Loc - x, height, x,
				                     y);
		} else if (wall_Loc - x >= 2 && height >= 2)
			generate_room_recdev(map_packed, wall_Loc - x, height, x,
			                     y);

		if (width - wall_Loc + x - 1 > 2 && height > 2) {
			if (rand() % 10 < 9
			        || width - wall_Loc + x - 1 > map_packed->width / 4
			        ||
			        height > map_packed->height / 4)
				build_wall_recdev(map_packed, width - wall_Loc + x - 1,
				                  height,
				                  height >= width - wall_Loc + x - 1, wall_Loc + 1,
				                  y);
			else if (width - wall_Loc + x - 1 >
			         map_packed->width / 20 ||
			         height > map_packed->height / 20)
				generate_room_recdev(map_packed, width - wall_Loc + x - 1,
				                     height, wall_Loc + 1,
				                     y);
		} else if (width - wall_Loc + x - 1 >= 2
		           && height > 2)
			generate_room_recdev(map_packed, width - wall_Loc + x - 1,
			                     height, wall_Loc + 1, y);

		int gate_Loc;
		int it = 0;
		int possible_Hole_Spots[height];

		for (int i = y; i < y + height; i++)
			if ((map[i][wall_Loc - 1].flags & FLAG_SOLID) == 0
			        &&
			        (map[i][wall_Loc + 1].flags & FLAG_SOLID) == 0) {
				possible_Hole_Spots[it] = i;
				it++;
			}

		if (it == 1)
			gate_Loc = possible_Hole_Spots[0];
		else
			gate_Loc = possible_Hole_Spots[rand() % it];

		map[gate_Loc][wall_Loc].symbol = '.' | COLOR_PAIR(
		                                   1);
		map[gate_Loc][wall_Loc].flags &= ~FLAG_SOLID;
	}
}

map_t* mapgen_recdev(map_t * map) {

	map->rooms = vector_init(1);

	UNPACK(map_unpacked, map);

	for (int i = 0; i < map->width; i++)
		for (int j = 0; j < map->height; j++) {
			map_unpacked[j][i].symbol = '.' | COLOR_PAIR(1);
			map_unpacked[j][i].flags &= ~FLAG_SOLID;
		}

	build_wall_recdev(map, map->width, map->height, rand() % 2,
	                  0, 0);

	return map;
}
