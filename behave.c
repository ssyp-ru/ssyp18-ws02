#include "behave.h"
#include "mapgen.h"

int behave_player(actor_t* self) {
	UNPACK(map, self->level->map);

	int key = getch();

	switch (key) {
	case KEY_F(2):
		return 0;

	case 'w':
		if (self->y < 1) break;

		if (map[self->y - 1][self->x].flags & FLAG_SOLID) break;

		self->y--;

		break;

	case 's':
		if (self->y + 1 >= self->level->map->height) break;

		if (map[self->y + 1][self->x].flags & FLAG_SOLID) break;

		self->y++;

		break;

	case 'a':
		if (self->x < 1) break;

		if (map[self->y][self->x - 1].flags & FLAG_SOLID) break;

		self->x--;

		break;

	case 'd':
		if (self->x + 1 >= self->level->map->width) break;
		if (map[self->y][self->x + 1].flags & FLAG_SOLID) break;

		self->x++;

		break;
	}

	return 1;
}
