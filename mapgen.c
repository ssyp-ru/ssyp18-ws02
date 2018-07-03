#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "mapgen.h"




/*void dfs(char m[MAZE_SIZE][MAZE_SIZE], int curX, int curY)
{
	m[curX][curY] = '.';
	if(curX - 1 >= 0 && m[curX - 1][curY] == '.')
	{
		dfs(m, curX - 1, curY);
	}
	if(curX + 1 < MAZE_SIZE && m[curX + 1][curY] == '.')
	{
		dfs(m, curX + 1, curY);
	}
	if(curY - 1 >= 0 && m[curX][curY - 1] == '.')
	{
		dfs(m, curX, curY - 1);
	}	
	if(curY + 1 < MAZE_SIZE && m[curX][curY + 1] == '.')
	{
		dfs(m, curX, curY + 1);
	}
}*/

void drawMap(map_t* mapPacked)
{
	UNPACK(map, mapPacked);
	

	for(int i = 0; i < mapPacked->width; i++)
	{
		for(int j = 0; j < mapPacked->height; j++)
		{
			mvaddch(j, i, map[j][i]);
		}
	}
	refresh();
}

void buildWall(map_t* mapPacked, int width, int height, bool isHorizontal, int x, int y)
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
			map[wallLoc][i] = '#';
//			mvaddch(wallLoc, i, '#');
		}
		if(width > 2 && wallLoc - y > 2 && (rand() % 15 < 13 || width > mapPacked->width / 5 || height > mapPacked->height / 5))	
			buildWall(mapPacked, width, wallLoc - y, wallLoc - y >= width, x, y);	
		if(width > 2 && height - wallLoc + y - 1 > 2 && (rand() % 15 < 13 || width > mapPacked->width / 5 || height > mapPacked->height / 5))
			buildWall(mapPacked, width, height - wallLoc + y - 1, height - wallLoc + y - 1 >= width, x, wallLoc + 1);
		int gateLoc;
		int it = 0;
		int possibleHoleSpots[width];
		for(int i = x; i < x + width; i++)
		{
			if(map[wallLoc - 1][i] == '.' && map[wallLoc + 1][i] == '.')
			{
				possibleHoleSpots[it] = i;
				it++;
			}
		}
		if(it == 1)
			gateLoc = possibleHoleSpots[0];
		else
			gateLoc = possibleHoleSpots[rand() % it];
  		map[wallLoc][gateLoc] = '.';
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
			map[i][wallLoc] = '#';
//			mvaddch(i, wallLoc, '#');
		}
		if(wallLoc - x > 2 && height > 2 && (rand() % 15 < 13 || width > mapPacked->width / 5 || height > mapPacked->height / 5))
			buildWall(mapPacked, wallLoc - x, height, height >= wallLoc - x, x, y);

		if(width - wallLoc + x - 2 > 2 && height > 2 && (rand() % 15 < 13 || width > mapPacked->width / 5 || height > mapPacked->height / 5))
			buildWall(mapPacked, width - wallLoc + x - 1, height, height >= width - wallLoc + x - 1, wallLoc + 1, y);
		int gateLoc;
		int it = 0;
		int possibleHoleSpots[height];	
		for(int i = y; i < y + height; i++)
		{
			if(map[i][wallLoc - 1] == '.' && map[i][wallLoc + 1] == '.')
			{
				possibleHoleSpots[it] = i;
				it++;
			}
			
		}
		if(it == 1)
			gateLoc = possibleHoleSpots[0];
		else
			gateLoc = possibleHoleSpots[rand() % it];
		map[gateLoc][wallLoc] = '.';	
	}
	
}

