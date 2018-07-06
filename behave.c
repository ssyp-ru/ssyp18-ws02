#include "behave.h"
#include "mapgen.h"


int behave_player(actor_t* self)
{
	UNPACK(map, self->level->map);
	
	int key = getch();

	switch(key)
	{
		case KEY_F(2):	
			return 0;
		
		case 'w':
			if (self->y - 1 >= 0 &&
			(map[self->y  - 1][self->x].flags & FLAG_SOLID) == 0)
				self->y--;
			break;
		case 's':
			if((self->y + 1 < self->level->map->height) &&
				(map[self->y + 1][self->x].flags & FLAG_SOLID) == 0)
				self->y++;
			break;
	
		case 'a':
			if((self->x - 1) >= 0 &&
				(map[self->y][self->x - 1].flags & FLAG_SOLID) == 0)
				self->x--;
			break;

		case 'd':
			if((self->x + 1 < self->level->map->width) &&
				(map[self->y][self->x + 1].flags & FLAG_SOLID) == 0)
				self->x++;
			break;
	}

	return 1;
}

