#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include "actor.h"
#include "mapgen.h"

void behave_fire(actor_t* self, level_t* level){
	actor_t bullet;
	bullet.health = 1;
	bullet.speed = 1;
	bullet.painchance = 0;
	bullet.view = '*';
	switch(self->direct){
		case 0:
			bullet.x = self->x + 1;
			bullet.y = self->y;
			bullet.direct = 0;
		break;
		case 1:
			bullet.x = self->x + 1;
			bullet.y = self->y + 1;
			bullet.direct = 1;
		break;
		case 2:
			bullet.x = self->x;
			bullet.y = self->y + 1;
			bullet.direct = 2;
		break;
		case 3:
			bullet.x = self->x - 1;
			bullet.y = self->y + 1;
			bullet.direct = 3;
		break;
		case 4:
			bullet.x = self->x - 1;
			bullet.y = self->y;
			bullet.direct = 4;
		break;
		case 5:
			bullet.x = self->x - 1;
			bullet.y = self->y - 1;
			bullet.direct = 5;
		break;
		case 6:
			bullet.x = self->x;
			bullet.y = self->y - 1;
			bullet.direct = 6;
		break;
		case 7:
			bullet.x = self->x + 1;
			bullet.y = self->y - 1;
			bullet.direct = 7;
		break;
	}
	bullet.inventory = NULL;
	//bullet behave_projectiles;
	//bullet create id
	bullet.targx = 0;
	bullet.targy = 0;
	vectorAddNewElem(level->actors, bullet);
}

void behave_projectiles(actor_t* self, level_t* level){
	UNPACK(map, level->map);
	switch(self->direct){
		case 0:
			self->x += 1;
		break;
		case 1:
			self->x += 1;
			self->y += 1;
		break;
		case 2:
			self->y += 1;
		break;
		case 3:
			self->x -= 1;
			self->y += 1;
		break;
		case 4:
			self->x -= 1;
		break;
		case 5:
			self->x -= 1;
			self->y -= 1;
		break;
		case 6:
			self->y -= 1;
		break;
		case 7:
			self->x += 1;
			self->y -= 1;
		break;
	}
	if ((map[self->x][self->y].flags & FLAG_SOLID) != 0){
		int i = 0;
		for (i = 0; self->id != level->actor->allActor[i].id; i++);
		deleteElem(level->actor, i);
	}
	for (int i = 0; i < level->actors->len; i++){
		if (level->actors->allActors[i].x == self->x
				&& level->actors->allActors[i].y == self-> y
				&& self->id != level->actors->allActors[i].id){
			level->actors->allActors[i]->health -= 1;
			int j = 0;
			for (j = 0; self->id != level->actor->allActor[i].id; j++);
			deleteElem(level->actor, j);
		}
	}
}

void behave_searchtarget(actor_t* self, level_t* level){
	char flag = 'a';
	for (int i = 0; i < level->actors->len; i++){
		if (abs(level->actors->allActors[i].x - self->x) <= 3
				&& abs(level->actors->allActors[i].y - self->y) <= 3
				&& self->id != level->actors->allActors[i].id){
			self->targx = level->actors->allActors[i].x;
			self->targy = level->actors->allActors[i].y;
			//self->behave = behave_chasetarget;
			flag = 'b';
		}
	}
	if (flag != 'b')//and flag can "walking"){ //if target not found
		int i = rand() % 8;	//actor walks and search target
		switch(i){
			case 0:
				self->x += 1;
				self->direct = 0;
			break;
			case 1:
				self->x += 1;
				self->y += 1;
				self->direct = 1;
			break;
			case 2:
				self->y += 1;
				self->direct = 2;
			break;
			case 3:
				self->x -= 1;
				self->y += 1;
				self->direct = 3;
			break;
			case 4:
				self->x -= 1;
				self->direct = 4;
			break;
			case 5:
				self->x -= 1;
				self->y -= 1;
				self->direct = 5;
			break;
			case 6:
				self->y -= 1;
				self->direct = 6;
			break;
			case 7:
				self->y -= 1;
				self->x += 1;
				self->direct = 7;
			break;
		}
	}
}

void behave_chasetarget(actor_t* self, level_t* level){
	if (self->x != self->targx){
		self->x += (self->x - self->targx)/abs(self->x - self->targx);
	}
	if (self->y != self->targy){
		self->y += (self->y - self->targy)/abs(self->x - self->targy);
	}
	for (int i = 0; i < level->actors->len; i++){
		if (abs(level->actors->allActors[i].x - self->x) <= 3
				&& abs(level->actors->allActors[i].y - self->y) <= 3
				&& self->id != level->actors->allActors[i].id){
			self->targx = level->actors->allActors[i].x;
			self->targy = level->actors->allActors[i].y;
			if (abs(level->actors->allActors[i].x - self->x) <= 3
				&& abs(level->actors->allActors[i].y - self->y) <= 3
				&& self->id != level->actors->allActors[i].id){
				int r = rand() % 2;
				if (r == 0){
					//self->behave = behave_meleeAttack
				}
				if (r == 1){
					//self->behave = behave_fire
				}
			}
			flag = 'b';
		}
	}
	if (flag != 'b'){
		//self->behave=searchTarget
	}
}

void behave_meleeattack(actor_t* self, level_t* level){
	for (int i = 0; i < level->actors->len; i++){
		if (abs(level->actors->allActors[i].x - self->x) <= 1
				&& abs(level->actors->allActors[i].y - self->y) <= 1
				&& self->id != level->actors->allActors[i].id){
			level->actors->allActors[i].health -= 1;
			//self->behave = behave_chasetarget;
			flag = 'b';
		}
	}
}

void behave_pain(actor_t* self, level_t* level){
	UNPACK(map, level->map);
	int min = 2 << 31; 
	int mini = 0;
	for (int i = 0; i < level->actors->len; i++){
		if (abs(level->actors->allActors[i].x - self->x) + 
				abs(level->actors->allActors[i].y - self->y) < min
				&& self->id != level->actors->allActors[i].id){
			min = abs(level->actors->allActors[i].x - self->x)\
			+ abs(level->actors->allActors[i].y - self->y);
			mini = i;
		}
	}
	int x -= (level->actors->allActors[i].x - self->x)\
	/ abs(level->actors->allActors[i].x - self.x);
	int y -= (level->actors->allActors[i].y - self->y)\
	/ abs(level->actors->allActors[i].y - self.y);
	if (!(map[self->x + x][self->y].flag&FLAG_SOLID)){
		self->x += x;
	}
	if (!(map[self->x][self->y + y].flag&FLAG_SOLID)){
		self->y += y;
	}
}
