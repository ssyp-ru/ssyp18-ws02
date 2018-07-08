#include "mapgen.h"

void generate_room(map_t* map_packed, int width,
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

void build_wall(map_t* map_packed,
                int width,
                int height,
                bool isHorizontal,
                int x,
                int y) {
	UNPACK(map, map_packed);

	if (isHorizontal) {
		int wallLoc;

		if (height == 3)
			wallLoc = y + 1;
		else
			wallLoc = rand() % (height - 2) + 1 + y;

		for (int i = x; i < x + width; i++) {
			map[wallLoc][i].symbol = '#' | COLOR_PAIR(1);
			map[wallLoc][i].flags |= FLAG_SOLID;
		}

		if (width > 2 && wallLoc - y > 2) {
			if (rand() % 10 < 9
			        || width > map_packed->width / 4 ||
			        wallLoc - y > map_packed->height / 4) {
				build_wall(map_packed, width, wallLoc - y,
				           wallLoc - y >= width, x, y);
			} else if (width > map_packed->width / 20 ||
			           wallLoc - y > map_packed->height / 20)
				generate_room(map_packed, width, wallLoc - y, x, y);
		} else if (width >= 2 && wallLoc - y >= 2)
			generate_room(map_packed, width, wallLoc - y, x, y);

		if (width > 2 && height - wallLoc + y - 1 > 2) {
			if (rand() % 10 < 9
			        || width > map_packed->width / 4 ||
			        height - wallLoc + y - 1 > map_packed->height / 4)
				build_wall(map_packed, width,
				           height - wallLoc + y - 1,
				           height - wallLoc + y - 1 >= width, x,
				           wallLoc + 1);
			else if (width > map_packed->width / 20 ||
			         height - wallLoc + y - 1 > map_packed->height / 20)
				generate_room(map_packed, width,
				              height - wallLoc + y - 1, x,
				              wallLoc + 1);
		} else if (width >= 2
		           && height - wallLoc + y - 1 >= 2)
			generate_room(map_packed, width,
			              height - wallLoc + y - 1, x, wallLoc + 1);

		int gateLoc;
		int it = 0;
		int possibleHoleSpots[width];

		for (int i = x; i < x + width; i++)
			if ((map[wallLoc - 1][i].flags & FLAG_SOLID) == 0
			        &&
			        (map[wallLoc + 1][i].flags & FLAG_SOLID) == 0) {
				possibleHoleSpots[it] = i;
				it++;
			}

		if (it == 1)
			gateLoc = possibleHoleSpots[0];
		else
			gateLoc = possibleHoleSpots[rand() % it];

		map[wallLoc][gateLoc].symbol = '.' | COLOR_PAIR(
		                                   1);
		map[wallLoc][gateLoc].flags &= ~FLAG_SOLID;
	} else {
		int wallLoc;

		if (width == 3)
			wallLoc = x + 1;
		else
			wallLoc = rand() % (width - 2) + 1 + x;

		for (int i = y; i < y + height; i++) {
			map[i][wallLoc].symbol = '#' | COLOR_PAIR(1);
			map[i][wallLoc].flags ^= FLAG_SOLID;
		}
		if (wallLoc - x > 2 && height > 2) {
			if (rand() % 10 < 9
			        || wallLoc - x > map_packed->width / 4 ||
			        height > map_packed->height / 4)
				build_wall(map_packed, wallLoc - x, height,
				           height >= wallLoc - x, x, y);
			else if (wallLoc - x > map_packed->width / 20 ||
			         height > map_packed->height / 20)
				generate_room(map_packed, wallLoc - x, height, x,
				              y);
		} else if (wallLoc - x >= 2 && height >= 2)
			generate_room(map_packed, wallLoc - x, height, x,
			              y);

		if (width - wallLoc + x - 1 > 2 && height > 2) {
			if (rand() % 10 < 9
			        || width - wallLoc + x - 1 > map_packed->width / 4
			        ||
			        height > map_packed->height / 4)
				build_wall(map_packed, width - wallLoc + x - 1,
				           height,
				           height >= width - wallLoc + x - 1, wallLoc + 1,
				           y);
			else if (width - wallLoc + x - 1 >
			         map_packed->width / 20 ||
			         height > map_packed->height / 20)
				generate_room(map_packed, width - wallLoc + x - 1,
				              height, wallLoc + 1,
				              y);
		} else if (width - wallLoc + x - 1 >= 2
		           && height > 2)
			generate_room(map_packed, width - wallLoc + x - 1,
			              height, wallLoc + 1, y);

		int gateLoc;
		int it = 0;
		int possibleHoleSpots[height];

		for (int i = y; i < y + height; i++)
			if ((map[i][wallLoc - 1].flags & FLAG_SOLID) == 0
			        &&
			        (map[i][wallLoc + 1].flags & FLAG_SOLID) == 0) {
				possibleHoleSpots[it] = i;
				it++;
			}

		if (it == 1)
			gateLoc = possibleHoleSpots[0];
		else
			gateLoc = possibleHoleSpots[rand() % it];

		map[gateLoc][wallLoc].symbol = '.' | COLOR_PAIR(
		                                   1);
		map[gateLoc][wallLoc].flags &= ~FLAG_SOLID;
	}
}

map_t* generate_map(int width, int height) {
	map_t* map_packed = (map_t*)malloc(sizeof(map_t));
	map_packed->buffer = (tile_t*)calloc(
	                         width * height, sizeof(tile_t));
	map_packed->width = width;
	map_packed->height = height;
	map_packed->rooms = vector_init(1);

	UNPACK(map, map_packed);

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			map[j][i].symbol = '.' | COLOR_PAIR(1);
			map[j][i].flags &= ~FLAG_SOLID;
		}

	build_wall(map_packed, width, height, rand() % 2, 0,
	           0);

	return map_packed;
}

void draw_map(map_t* map_packed) {
	UNPACK(map, map_packed);

	for (int i = 0; i < map_packed->width; i++) {
		for (int j = 0; j < map_packed->height; j++) {
			mvaddch(j, i, map[j][i].symbol);
		}
	}
}

void free_map(map_t* map) {
	vector_free(map->rooms);
	free(map->buffer);
	free(map);
}
