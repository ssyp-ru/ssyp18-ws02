#include "behave.h"
#include "find_path.h"
#include "actor.h"

int behave_player(actor_t * self) {
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

int behave_goblin(actor_t * self){
	actor_t * find_actor =	actor_get(self->level->actors, 0);
	int x = find_actor->x;
	int y = find_actor->y;
	int xm = self->x - x;
	int ym = self->y - y;
	int vr = self->view_radius;
	pvector_t * path;
	if(xm * xm + ym * ym < vr * vr){
		path = find_path(self, x, y);
		if(path->length > 0){
			int x1 = path->buffer[path->length].x;
			int y1 = path->buffer[path->length].y;
			self->x = x1;
			self->y = y1;
		}
	}
	return 1;
}
