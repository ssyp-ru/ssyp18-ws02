#include <ncurses.h>
#include <stdlib.h>
#include "mapgen.h"
#include "common.h"
#include "roomvector.h"
#include "room.h"

void generateRoom(map_t* mapPacked, int width, int height, int x, int y)
{
	UNPACK(map, mapPacked);

	for(int i = x; i < x + width; i++)
	{
		for(int j = y; j < y + height; j++)
		{
			map[j][i].symbol = ' ' | COLOR_PAIR(1);
			map[j][i].flags |= FLAG_PERM;			
		}
	}

	room_t newRoom;
	newRoom.x = x;
	newRoom.y = y;
	newRoom.width = width;
	newRoom.height = height;

	vector_add(mapPacked->rooms, newRoom);
}


void buildWall(map_t* mapPacked, int width, int height, 
							bool isHorizontal, int x, int y)
{
	UNPACK(map, mapPacked);

	if(isHorizontal)
	{
		int wallLoc;

		if(height == 3)
			wallLoc = y + 1;
		else
			wallLoc = rand() % (height - 2) + 1 + y;

		for(int i = x; i < x + width; i++)
		{
			map[wallLoc][i].symbol = '#' | COLOR_PAIR(1);
			map[wallLoc][i].flags &= ~FLAG_PERM;
		}

		if(width > 2 && wallLoc - y > 2)
		{
			if(rand() % 10 < 9 || width > mapPacked->width / 4 
			|| wallLoc - y > mapPacked->height / 4)
			{
				buildWall(mapPacked, width, wallLoc - y,
				wallLoc - y >= width, x, y);
			}
			else if(width > mapPacked->width / 20
			|| wallLoc - y > mapPacked->height / 20)
				generateRoom(mapPacked, width, wallLoc - y, x, y);
		}
		else if(width >= 2 && wallLoc - y >= 2)
			generateRoom(mapPacked, width, wallLoc - y, x, y);

		if(width > 2 && height - wallLoc + y - 1 > 2)
		{
			if(rand() % 10 < 9 || width > mapPacked->width / 4
			|| height - wallLoc + y - 1 > mapPacked->height / 4)
				buildWall(mapPacked, width, height - wallLoc + y - 1,
				height - wallLoc + y - 1 >= width, x, wallLoc + 1);
			else if(width > mapPacked->width / 20 
			|| height - wallLoc + y - 1 > mapPacked->height / 20)
				generateRoom(mapPacked, width,
				height - wallLoc + y - 1, x, wallLoc + 1);
		}
		else if(width >= 2 && height - wallLoc + y - 1 >= 2)
			generateRoom(mapPacked, width, 
			height - wallLoc + y - 1, x, wallLoc + 1);

		int gateLoc;
		int it = 0;
		int possibleHoleSpots[width];

		for(int i = x; i < x + width; i++)
			if((map[wallLoc + 1][i].flags & FLAG_PERM) != 0 
			&& (map[wallLoc + 1][i].flags & FLAG_PERM) != 0)
			{
				possibleHoleSpots[it] = i;
				it++;
			}

		if(it == 1)
			gateLoc = possibleHoleSpots[0];
		else
			gateLoc = possibleHoleSpots[rand() % it];
		
		map[wallLoc][gateLoc].symbol = '.' | COLOR_PAIR(1);
		map[wallLoc][gateLoc].flags |= FLAG_PERM; 
	}
	else
	{
		int wallLoc;

		if(width == 3)
			wallLoc = x + 1;
		else
			wallLoc = rand() % (width - 2) + 1 + x;

		for(int i = y; i < y + height; i++)
		{
			map[i][wallLoc].symbol = '#' | COLOR_PAIR(1);
			map[i][wallLoc].flags ^= FLAG_PERM;
		}
		if(wallLoc - x > 2 && height > 2)
		{
			if(rand() % 10 < 9 || wallLoc - x > mapPacked->width / 4
			|| height > mapPacked->height / 4)
				buildWall(mapPacked, wallLoc - x,
				height, height >= wallLoc - x, x, y);
			else if(wallLoc - x > mapPacked->width / 20
			|| height > mapPacked->height / 20)
				generateRoom(mapPacked, wallLoc - x, height, x, y);
		}
		else if(wallLoc - x >= 2 && height >= 2)
			generateRoom(mapPacked, wallLoc - x, height, x, y);

		if(width - wallLoc + x - 1 > 2 && height > 2)
		{
			if(rand() % 10 < 9 || width - wallLoc + x - 1 > mapPacked->width / 4
			|| height > mapPacked->height / 4)
				buildWall(mapPacked, width - wallLoc + x - 1, height, 
				height >= width - wallLoc + x - 1, wallLoc + 1, y);
			else if(width - wallLoc + x - 1 > mapPacked->width / 20
			|| height > mapPacked->height / 20)
				generateRoom(mapPacked, width - wallLoc + x - 1, 
										height, wallLoc + 1, y);
		}
		else if(width - wallLoc + x - 1 >= 2 && height > 2)
			generateRoom(mapPacked, width - wallLoc + x - 1,
			height, wallLoc + 1, y);

		int gateLoc;
		int it = 0;
		int possibleHoleSpots[height];

		for(int i = y; i < y + height; i++)
			if((map[i][wallLoc - 1].flags & FLAG_PERM) != 0
			&& (map[i][wallLoc + 1].flags & FLAG_PERM) != 0)
			{
				possibleHoleSpots[it] = i;
				it++;
			}
		
		if(it == 1)
			gateLoc = possibleHoleSpots[0];
		else
			gateLoc = possibleHoleSpots[rand() % it];

		map[gateLoc][wallLoc].symbol = '.' | COLOR_PAIR(1);
		map[gateLoc][wallLoc].flags |= FLAG_PERM;
	}

}


map_t* generateMap(int width, int height)
{
	map_t* mapPacked = (map_t*) malloc(sizeof(map_t));
	mapPacked->buffer = (tile_t*) calloc(width * height, sizeof(tile_t));
	mapPacked->width = width;
	mapPacked->height = height;
	mapPacked->amountOfRooms = 0;
	mapPacked->rooms = vector_init(1);

	UNPACK(map, mapPacked);

	for(int i = 0 ; i < width; i++)
		for(int j = 0; j < height; j++)
		{
			map[j][i].symbol = '.' | COLOR_PAIR(1);
			map[j][i].flags |= FLAG_PERM;
		}

	buildWall(mapPacked, width, height, rand() % 2, 0, 0);

	return mapPacked;
}


void drawMap(map_t* mapPacked)
{
	UNPACK(map, mapPacked);

	for(int i = 0; i < mapPacked->width; i++)
	{
		for(int j = 0; j < mapPacked->height; j++)
		{
			mvaddch(j, i, map[j][i].symbol);
		}
	}
}

void freeMap(map_t* map)
{
	vector_free(map->rooms);
	free(map->buffer);
	free(map);
}
