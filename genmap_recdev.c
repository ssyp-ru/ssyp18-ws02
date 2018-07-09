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
  // Effing abomination.
	if (is_horizontal) {
		int wall_loc;

		if (height == 3)
			wall_loc = y + 1;
		else
			wall_loc = rand() % (height - 2) + 1 + y;

		for (int i = x; i < x + width; i++) {
			map[wall_loc][i].symbol = '#' | COLOR_PAIR(1);
			map[wall_loc][i].flags |= FLAG_SOLID;
		}

		if (width > 2 && wall_loc - y > 2) {
			if (rand() % 10 < 9
			        || width > 20 ||
			        wall_loc - y > 20) {
				build_wall_recdev(map_packed, width, wall_loc - y,
				                  wall_loc - y >= width, x, y);
			} else 
				generate_room_recdev(map_packed, width, wall_loc - y, x, y);
		} else if (width >= 2 && wall_loc - y >= 2)
			generate_room_recdev(map_packed, width, wall_loc - y, x, y);

		if (width > 2 && height - wall_loc + y - 1 > 2) {
			if (rand() % 10 < 9
			        || width > 20 ||
			        height - wall_loc + y - 1 > 20)
				build_wall_recdev(map_packed, width,
				                  height - wall_loc + y - 1,
				                  height - wall_loc + y - 1 >= width, x,
				                  wall_loc + 1);
			else 
				generate_room_recdev(map_packed, width,
				                     height - wall_loc + y - 1, x,
				                     wall_loc + 1);
		} else if (width >= 2
		           && height - wall_loc + y - 1 >= 2)
			generate_room_recdev(map_packed, width,
			                     height - wall_loc + y - 1, x, wall_loc + 1);

		int gate_loc;
		int it = 0;
		int possible_hole_spots[width];

		for (int i = x; i < x + width; i++)
			if ((map[wall_loc - 1][i].flags & FLAG_SOLID) == 0
			        &&
			        (map[wall_loc + 1][i].flags & FLAG_SOLID) == 0) {
				possible_hole_spots[it] = i;
				it++;
			}

		if (it == 1)
			gate_loc = possible_hole_spots[0];
		else
			gate_loc = possible_hole_spots[rand() % it];

		map[wall_loc][gate_loc].symbol = ' ' | COLOR_PAIR(
		                                   1);
		map[wall_loc][gate_loc].flags &= ~FLAG_SOLID;
	} else {
		int wall_loc;

		if (width == 3)
			wall_loc = x + 1;
		else
			wall_loc = rand() % (width - 2) + 1 + x;

		for (int i = y; i < y + height; i++) {
			map[i][wall_loc].symbol = '#' | COLOR_PAIR(1);
			map[i][wall_loc].flags ^= FLAG_SOLID;
		}
		if (wall_loc - x > 2 && height > 2) {
			if (rand() % 10 < 9
			        || wall_loc - x > 20 ||
			        height > 20)
				build_wall_recdev(map_packed, wall_loc - x, height,
				                  height >= wall_loc - x, x, y);
			else 
				generate_room_recdev(map_packed, wall_loc - x, height, x,
				                     y);
		} else if (wall_loc - x >= 2 && height >= 2)
			generate_room_recdev(map_packed, wall_loc - x, height, x,
			                     y);

		if (width - wall_loc + x - 1 > 2 && height > 2) {
			if (rand() % 10 < 9
			        || width - wall_loc + x - 1 > 20
			        ||
			        height > 20)
				build_wall_recdev(map_packed, width - wall_loc + x - 1,
				                  height,
				                  height >= width - wall_loc + x - 1, wall_loc + 1,
				                  y);
			else 
				generate_room_recdev(map_packed, width - wall_loc + x - 1,
				                     height, wall_loc + 1,
				                     y);
		} else if (width - wall_loc + x - 1 >= 2
		           && height > 2)
			generate_room_recdev(map_packed, width - wall_loc + x - 1,
			                     height, wall_loc + 1, y);

		int gate_loc;
		int it = 0;
		int possible_hole_spots[height];

		for (int i = y; i < y + height; i++)
			if ((map[i][wall_loc - 1].flags & FLAG_SOLID) == 0
			        &&
			        (map[i][wall_loc + 1].flags & FLAG_SOLID) == 0) {
				possible_hole_spots[it] = i;
				it++;
			}

		if (it == 1)
			gate_loc = possible_hole_spots[0];
		else
			gate_loc = possible_hole_spots[rand() % it];

		map[gate_loc][wall_loc].symbol = ' ' | COLOR_PAIR(
		                                   1);
		map[gate_loc][wall_loc].flags &= ~FLAG_SOLID;
	}
}

map_t* mapgen_recdev(map_t * map) {
//	if(map->rooms != NULL)
	map->rooms = vector_init(1);

	UNPACK(map_unpacked, map);

	for (int i = 0; i < map->width; i++)
		for (int j = 0; j < map->height; j++) {
			map_unpacked[j][i].symbol = ' ' | COLOR_PAIR(1);
			map_unpacked[j][i].flags &= ~FLAG_SOLID;
		}

	build_wall_recdev(map, map->width, map->height, rand() % 2,
	                  0, 0);

	return map;
}
