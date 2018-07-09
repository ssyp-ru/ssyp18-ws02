#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "game.h"
#include <stdint.h>
#include "actor.h"
#include "behave.h"

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
	actor_t* bullet = (actor_t*)calloc(1, sizeof(actor_t));
	bullet->hp = 1;
	bullet->symbol = '*' | COLOR_PAIR(2);
	switch(self->direct){
		case 0:
			bullet->x = self->x + 1;
			bullet->y = self->y;
			bullet->direct = 0;
		break;
		case 1:
			bullet->x = self->x - 1;
			bullet->y = self->y;
			bullet->direct = 1;
		break;
		case 2:
			bullet->x = self->x;
			bullet->y = self->y + 1;
			bullet->direct = 2;
		break;
		case 3:
			bullet->x = self->x;
			bullet->y = self->y - 1;
			bullet->direct = 3;
		break;
	}
	bullet->inventory = NULL;
	bullet->behave = behave_projectiles;
	bullet->id = give_id();
	bullet->targ_x = 0;
	bullet->targ_y = 0;
	bullet->flags = 0;
	bullet->flags |= FLAG_PROJECTILE;
	bullet->level = self->level;
	add_vector_elem(self->level->actors, bullet);
	add_actor(bullet);
	self->state = 0;
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
	if (self->level->actors != NULL){
		for (int i = 0; i < self->level->actors->length; i++){
			//------------------------------------------------------
			if (self->level->actors->data[i]->x == self->x
				&& self->level->actors->data[i]->y == self-> y
				&& self->id != self->level->actors->data[i]->id
				&& !(self->level->actors->data[i]->flags & FLAG_PROJECTILE)){
			//------------------------------------------------------
				self->level->actors->data[i]->hp -= 3;

				if (self->level->actors->data[i]->hp <= 0){
					self->level->actors->data[i]->flags |= FLAG_DEAD;
				}
				self->flags |= FLAG_DEAD;			
			}
		}
	}
	if (self->x < 0 || self->y < 0 || self->x > self->level->map->width
			|| self->y > self->level->map->height){
		self->flags |= FLAG_DEAD;
		return 1;
	}
	if (map[self->y][self->x].flags & FLAG_SOLID){
		self->flags |= FLAG_DEAD;
	}
	return 1;
}

int behave_searchtarget(actor_t* self){
	UNPACK(map, self->level->map);
	int flag = 0;
	if (self->level->actors != NULL){
		for (int i = 0; i < self->level->actors->length; i++){
			//------------------------------------------------------
			if(abs(self->level->actors->data[i]->x - self->x) <= self->view_radius
			&& abs(self->level->actors->data[i]->y - self->y) <= self->view_radius
			&& self->id != self->level->actors->data[i]->id
			&& !(self->level->actors->data[i]->flags & FLAG_PROJECTILE)){
			//------------------------------------------------------
				self->targ_x = self->level->actors->data[i]->x;
				self->targ_y = self->level->actors->data[i]->y;
				self->state = 1;
				flag = 1;
			}
		}
	}
	if (flag != 1 && (self->flags & FLAG_CANWALK)){
		int i = rand() % 4;
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
	if(self->level->actors != NULL){
		for (int i = 0; i < self->level->actors->length; i++){
			//------------------------------------------------------
			if (abs(self->level->actors->data[i]->x - self->x) <= 1
				&& abs(self->level->actors->data[i]->y - self->y) <= 1
				&& self->id != self->level->actors->data[i]->id
				&& !(self->level->actors->data[i]->flags & FLAG_PROJECTILE)){
			//------------------------------------------------------
				self->level->actors->data[i]->hp -= 1;
				if (self->level->actors->data[i]->hp <= 0){
					self->level->actors->data[i]->flags |= FLAG_DEAD;
				}
			}
		}
	}
	self->state = 0;
	return 4;
}

int behave_chasetarget(actor_t* self){
	UNPACK(map, self->level->map);
	init_pair(50, COLOR_RED, COLOR_BLACK);
	self->symbol = 'O' | COLOR_PAIR(50);
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
	if (self->level->actors){
		for (int i = 0; i < self->level->actors->length; i++){
			//------------------------------------------------------
			if (abs(self->level->actors->data[i]->x - self->x) <= self->view_radius
				&& abs(self->level->actors->data[i]->y - self->y) <= self->view_radius
				&& self->id != self->level->actors->data[i]->id
				&& !(self->level->actors->data[i]->flags & FLAG_PROJECTILE)){
			//------------------------------------------------------
				self->targ_x = self->level->actors->data[i]->x;
				self->targ_y = self->level->actors->data[i]->y;
			//------------------------------------------------------
				if (self->flags & FLAG_MELEE){
					self->state = 2;
				} else {
					self->state = 3;
				}
				flag = 'b';
			}
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
			if (map[self->y - 1][self->x].flags & FLAG_SOLID)
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
