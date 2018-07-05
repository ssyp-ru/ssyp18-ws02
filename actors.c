#include "common.h"
#include <ncurses.h>
#include <stdlib.h>
#include "mapgen.h"
#include "actors.h"
#include "roomvector.h"

actors_t* initActors(map_t* mapPacked, int amountOfEntities)
{
	actors_t* actors = (actors_t*) malloc(sizeof(actors_t));
	actors->array = (actor_t*) calloc(amountOfEntities + 1, sizeof(actor_t));
	actors->amount = amountOfEntities + 1;
	
	int roomNumber = rand() % mapPacked->rooms->length;
	actors->array[0].symbol = '@' | COLOR_PAIR(1);
	actors->array[0].x = vector_get(mapPacked->rooms, roomNumber).x
	+ vector_get(mapPacked->rooms, roomNumber).width / 2;
	actors->array[0].y = vector_get(mapPacked->rooms, roomNumber).y
	+ vector_get(mapPacked->rooms, roomNumber).height / 2;
	actors->array[0].flags |= FLAG_ISPLAYER;

	return actors;	
}


bool behaveActors(map_t* mapPacked, actors_t* actors)
{
	UNPACK(map, mapPacked);
	
	for(int i = 0; i < actors->amount; i++)
		if((actors->array[i].flags & FLAG_ISPLAYER) != 0)
		{
			int key = getch();

			switch(key)
			{
				case KEY_F(2):
				  	
		 			return false;

				case 'w':
					if(actors->array[i].y - 1 >= 0
					&& (map[actors->array[i].y - 1][actors->array[i].x].flags
					& FLAG_PERM) != 0) 
					actors->array[i].y--;
					break;
	
				case 's':
					if(actors->array[i].y + 1 < mapPacked->height
					&& (map[actors->array[i].y + 1][actors->array[i].x].flags
					& FLAG_PERM) != 0)
						actors->array[i].y++;
					break;
	
				case 'a':
					if(actors->array[i].x - 1 >= 0
					&& (map[actors->array[i].y][actors->array[i].x - 1].flags
					& FLAG_PERM) != 0)
						actors->array[i].x--;
					break;
	
				case 'd':
					if(actors->array[i].x + 1 < mapPacked->width
					&& (map[actors->array[i].y][actors->array[i].x + 1].flags
					& FLAG_PERM) != 0)
						actors->array[i].x++;
					break;
			}
		}
	return true;

}


void drawActors(actors_t* actors)
{
	for(int i = 0; i < actors->amount; i++)
	{
		mvaddch(actors->array[i].y, actors->array[i].x,
		actors->array[i].symbol);
	}
}


void freeActors(actors_t* actors)
{
	free(actors->array);
	free(actors);
}
