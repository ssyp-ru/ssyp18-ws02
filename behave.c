#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include "actor.h"
#include "behave.h"
#include "mapgen.h"
#include "find_path.h"

int behave_monster(actor_t* self){
	int funcreturn = 0;
	switch(self->state){
		case 0:
			funcreturn = behave_searchtarget(self);
		break;
		case 1:
			funcreturn = behave_chasetarget(self);
		break;
		case 2:
			funcreturn = behave_meleeattack(self);
		break;
		case 3:
			funcreturn = behave_fire(self);
		break;
	}
	return funcreturn;
}

int behave_fire(actor_t* self){
	actor_t bullet;
	bullet.hp = 1;
	bullet.symbol = '*';
	switch(self->direct){
		case 0:
			bullet.x = self->x + 1;
			bullet.y = self->y;
			bullet.direct = 0;
		break;
		case 1:
			bullet.x = self->x + 1;
			bullet.y = self->y;
			bullet.direct = 1;
		break;
		case 2:
			bullet.x = self->x;
			bullet.y = self->y + 1;
			bullet.direct = 2;
		break;
		case 3:
			bullet.x = self->x;
			bullet.y = self->y - 1;
			bullet.direct = 3;
		break;
	}
	bullet.inventory = NULL;
	bullet.behave = behave_projectiles;
	bullet.id = self->level->actors->all_actors[self->level->actors->len - 1].id + 1;
	bullet.targ_x = 0;
	bullet.targ_y = 0;
	bullet.level = self->level;
	add_vector_elem(self->level->actors, bullet);
	self->state = 1;
	return 1;
}

int behave_projectiles(actor_t* self){
	UNPACK(map, self->level->map);
	switch(self->direct){
		case 0:
			self->x += 1;
		break;
		case 1:
			self->x -= 1;
		break;
		case 2:
			self->y += 1;
		break;
		case 3:
			self->y -= 1;
		break;
	}
	for (int i = 0; i < self->level->actors->len; i++){
		//------------------------------------------------------
		if (self->level->actors->all_actors[i].x == self->x
				&& self->level->actors->all_actors[i].y == self-> y
				&& self->id != self->level->actors->all_actors[i].id){
		//------------------------------------------------------
			self->level->actors->all_actors[i].hp -= 3;
			if (self->level->actors->all_actors[i].hp <= 0){
				self->level->actors->all_actors[i].flags |= FLAG_DEAD;
			}
			self->flags |= FLAG_DEAD;			
		}
	}
	if (self->x < 0 || self->y < 0 || self->x > self->level->map->width
			|| self->x > self->level->map->height){
		self->flags |= FLAG_DEAD;
		return 1;
	}
	if ((map[self->y][self->x].flags & FLAG_SOLID) != 0){
		self->flags |= FLAG_DEAD;
	}
	return 1;
}

int behave_searchtarget(actor_t* self){
	UNPACK(map, self->level->map);
	char flag = 'a';
	for (int i = 0; i < self->level->actors->len; i++){
		//------------------------------------------------------
		if(abs(self->level->actors->all_actors[i].x - self->x) <= self->view_radius
		&& abs(self->level->actors->all_actors[i].y - self->y) <= self->view_radius
		&& self->id != self->level->actors->all_actors[i].id){
		//------------------------------------------------------
			self->targ_x = self->level->actors->all_actors[i].x;
			self->targ_y = self->level->actors->all_actors[i].y;
			self->state = 1;
			flag = 'b';
		}
	}
	if (flag != 'b' && (self->flags & FLAG_CANWALK)){//and flag can "walking" //if target not found
		int i = rand() % 4;	//actor walks and search target
		switch(i){
			case 0:
				if (self->x + 1 >= self->level->map->width)
					break;
				if(map[self->y][self->x + 1].flags & FLAG_SOLID)
					break;
				self->x += 1;
				self->direct = 0;
			break;
			case 1:
				if (self->x - 1 >= self->level->map->width)
					break;
				if (map[self->y][self->x - 1].flags & FLAG_SOLID)
					break;
				self->x -= 1;
				self->direct = 1;
			break;
			case 2:
				if (self->y + 1 >= self->level->map->height)
					break;
				if (map[self->y + 1][self->x].flags & FLAG_SOLID)
					break;
				self->y += 1;
				self->direct = 2;
			break;
			case 3:
				if (self->y - 1 >= self->level->map->height)
					break;
				if (map[self->y - 1][self->x].flags & FLAG_SOLID)
					break;
				self->y -= 1;
				self->direct = 3;
			break;	
		}
	}
	return 3;
}

int behave_meleeattack(actor_t* self){
	for (int i = 0; i < self->level->actors->len; i++){
		//------------------------------------------------------
		if (abs(self->level->actors->all_actors[i].x - self->x) <= 1
				&& abs(self->level->actors->all_actors[i].y - self->y) <= 1
				&& self->id != self->level->actors->all_actors[i].id){
		//------------------------------------------------------
			self->level->actors->all_actors[i].hp -= 1;
			if (self->level->actors->all_actors[i].hp <= 0){
				self->level->actors->all_actors[i].flags |= FLAG_DEAD;
			}
			self->state = 1;
			return 4;
		}
	}
	self->state = 3;
	return 4;
}

int behave_chasetarget(actor_t* self){
	UNPACK(map, self->level->map);
	//pvector_t* cord = find_path(self, self->targ_x, self->targ_y);
	if (self->x != self->targ_x){
		if (self->x < self->targ_x){
			if (self->x + 1 < self->level->map->width
					&& !(map[self->y][self->x + 1].flags & FLAG_SOLID)){
				self->x += 1;
				self->direct = 0;
			}
		} else {
			if (self->x - 1 > 0
					&& !(map[self->y][self->x - 1].flags & FLAG_SOLID)){
				self->x -= 1;
				self->direct = 1;
			}
		}
	}
	if (self->y != self->targ_y){
		if (self->y < self->targ_y){
			if (self->y + 1 < self->level->map->height
					&& !(map[self->y + 1][self->x].flags & FLAG_SOLID)){
				self->y += 1;
				self->direct = 2;
			}
		} else {
			if (self->y - 1 > 0
					&& !(map[self->y - 1][self->x].flags & FLAG_SOLID)){
				self->y -= 1;
				self->direct = 3;
			}
		}
	}
	char flag = 'a';
	for (int i = 0; i < self->level->actors->len; i++){
		//------------------------------------------------------
		if (abs(self->level->actors->all_actors[i].x - self->x) <= 3
				&& abs(self->level->actors->all_actors[i].y - self->y) <= 3
				&& self->id != self->level->actors->all_actors[i].id){
		//------------------------------------------------------
			self->targ_x = self->level->actors->all_actors[i].x;
			self->targ_y = self->level->actors->all_actors[i].y;
		//------------------------------------------------------
			if (self->flags & FLAG_MELEE){
				self->state = 2;
			} else {
				self->state = 3;
			}
			flag = 'b';
		}
	}
	if (flag == 'a'){
		self->state = 0;
	}
	return 3;
}

int behave_player(actor_t* self){
	UNPACK(map, self->level->map);
	int key = getch();
	switch(key){
		case KEY_F(2):	
			return 0;
		break;
		case 'w':
			if (self->y < 1)
				break;
			if (map[self->y  - 1][self->x].flags & FLAG_SOLID)
				break;
			self->y--;
		break;
		case 's':
			if (self->y + 1 >= self->level->map->height)
				break;
			if (map[self->y + 1][self->x].flags & FLAG_SOLID)
				break;
			self->y++;
		break;
		case 'a':
			if (self->x < 1)
				break;
			if (map[self->y][self->x - 1].flags & FLAG_SOLID)
				break;
			self->x--;
		break;
		case 'd':
			if (self->x + 1 >= self->level->map->width)
				break;
	  	if (map[self->y][self->x + 1].flags & FLAG_SOLID)
				break;
		  self->x++;
		break;
	}
	return 3;
}
