#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>	
#include "actor.h"
#include "common.h"
#include "find_path.h"

pvector_t * find_path(actor_t * actor, int x, int y){
	UNPACK(map, actor->level->map);
	int size_x = actor->level->map->width;
	int size_y = actor->level->map->height;
	int vr = actor->view_radius;
	int x1 = actor->x;
	int y1 = actor->y;
	pvector_t * vector = calloc(1, sizeof(pvector_t));
	vector->buffer = calloc(vr * vr * 5, sizeof(pos_t));
	vector->capacity = vr * vr * 5;
	vector->length = 0;
	int set_map[size_y][size_x];
	for(int k = 0; k < size_y; k++){
		for(int i = 0; i < size_x; i++){
			set_map[k][i] = -1;
		}
	}
	set_map[y1][x1] = 0;
	int ra1 = 0;
	while(ra1 < 200){
		for(int k = y1 - vr; k <= y1 + vr && k > 0 && k < size_y - 1; k++){
			for(int i = x1 - vr; i <= x1 + vr && i > 0 && i < size_x - 1; i++){
				if(set_map[k][i] != - 1){
					if(set_map[k - 1][i] == -1 &&
						!(map[k - 1][i].flags & FLAG_TILE_SOLID))
							set_map[k - 1][i] = set_map[k][i] + 1;

					if(set_map[k + 1][i] == -1 && 
						!(map[k + 1][i].flags  & FLAG_TILE_SOLID))
							set_map[k + 1][i] = set_map[k][i] + 1;	

					if(set_map[k][i - 1] == -1 &&
						!(map[k][i - 1].flags  & FLAG_TILE_SOLID))
							set_map[k][i - 1] = set_map[k][i] + 1;	

					if(set_map[k][i + 1] == -1 &&
						!(map[k][i + 1].flags  & FLAG_TILE_SOLID))
							set_map[k][i + 1] = set_map[k][i] + 1;
				}
			}
		} 
		ra1++;
	}	
	//Find a way
	int ind = 0;
	int x_f = x, x_n;
	int y_f = y, y_n;
	int min = 999;
	for(int k = 0; k < size_y; k++){
		for(int i = 0; i < size_x; i++){
			if(set_map[k][i] == -1)
				set_map[k][i] = 999;
		}
	}
	bool visible = (x >= x1 - vr && x <= x1 + vr &&
			y >= y1 - vr && y <= y1 + vr);
	while(ind < 100 && visible){
		if(y_f + 1 < size_y){
			min = set_map[y_f + 1][x_f];
			x_n = x_f;
			y_n = y_f + 1;
		}
		if(y_f - 1 > 0){
			if(set_map[y_f - 1][x_f] < min){
				x_n = x_f;
				y_n = y_f - 1;
				min = set_map[y_f - 1][x_f];
			}	
		}	
		if(x_f + 1 < size_x){
			if(set_map[y_f][x_f + 1] < min){
				x_n = x_f + 1;
				y_n = y_f;
				min = set_map[y_f][x_f + 1];
			}
		}	
		if(x_f - 1 > 0){
			if(set_map[y_f][x_f - 1] < min){
				x_n = x_f - 1;
				y_n = y_f;
				min = set_map[y_f][x_f - 1];
			}
		}
		if(x_n == x1 && y_n == y1){
				break;
		}
		if(min == 999){
			ind = 0;
			vector->length = ind;
			break;
		}
		else{
			vector->buffer[ind].x = x_n;
			vector->buffer[ind].y = y_n;
			vector->length = ind;
			x_f = x_n;
			y_f = y_n;
			ind++;
		}
	}
	return vector;
}

