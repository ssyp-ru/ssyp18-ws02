#include <math.h>
#include "common.h"
#include "rooms.h"
#include <stdlib.h>
#include <ncurses.h>
void room_free(room_t* room) {
	if(room != NULL) free(room);
}
void createLine (int x1, int y1, int x2, int y2, char ch, int COLOR, map_t* map) {
	UNPACK(field, map);
	double x=x1, y=y1;
	if (x1-x2 == 0) {
		double l = 0;
		double c = x1;
		while (y != y2) {
			x = l*y + c;
			field[(int)y][(int)x].symbol = ch | COLOR_PAIR(COLOR);
			if (y2 >= y) {
				x++;
				y++;
			} else {
				x--;
				y--;
			}
		}
	} else {
		double k = (y1-y2)/(x1-x2);
		double b = y1 - k * x1;
		while (true) {
			if (x2 - x > y2 - y || y1 - y2 == 0) {
				y = k*x + b;
				x = round(x);
				y = round(y);
				field[(int)y][(int)x].symbol = ch | COLOR_PAIR(COLOR);
				if (x2 >= x) {
					if (x >= x2) break;
					x++;
					y++;
				} else {
					if (x <= x2) break;
					x--;
					y--;
				}
			} else {
				x = (y - b)/k;
				x = round(x);
				y = round(y);
				field[(int)y][(int)x].symbol = ch | COLOR_PAIR(COLOR);
				if (y2 >= y) {
					if (y >= y2) break;
					x++;
					y++;
				} else {
					if (y <= y2) break;
					x--;
					y--;
				}
			}
		}
	}
}
void divRoom (tree_t* parent, vector_t* leaves, map_t* map) {
	room_t* proom = parent->value;
	if(proom->S <= 64 || (proom->lenX <=16 && proom->lenY <= 8)) {
		vector_add(leaves, *parent);
		return;
	}
	room_t* nrooms[2];
	nrooms[0] = malloc(sizeof(room_t));
	nrooms[1] = malloc(sizeof(room_t));
	int lenLast;
	int x1=0, y1=0, x2=0, y2=0;
	if (proom->lenX > proom->lenY*2) {
		lenLast = proom->lenX;
		x1 = proom->x1 + (rand() %((lenLast*40)/100) + ((lenLast*30)/100));
		y1 = proom->y1;
		x2 = x1;
		y2 = proom->y2;
		createLine(x1, y1, x2, y2, '#', 1, map);
		nrooms[1]->x2 = proom->x2;
		nrooms[1]->y2 = proom->y2;
		nrooms[1]->x1 = x2;
		nrooms[1]->y1 = y1;
		nrooms[0]->x2 = x2;
		nrooms[0]->y2 = y2;
		nrooms[0]->x1 = proom->x1;
		nrooms[0]->y1 = proom->y1;
	} else {
		lenLast = proom->lenY;
		y2 = proom->y2 + (rand() %((lenLast*40)/100) + ((lenLast*30)/100));
		x2 = proom->x2;
		y1 = y2;
		x1 = x2 - proom->lenX;
		createLine(x1, y1, x2, y2, '#', 1, map);
		nrooms[1]->x2 = proom->x2;
		nrooms[1]->y2 = proom->y2;
		nrooms[1]->x1 = proom->x1;
		nrooms[1]->y1 = y2;
		nrooms[0]->x2 = x2;
		nrooms[0]->y2 = y2;
		nrooms[0]->x1 = proom->x1;
		nrooms[0]->y1 = proom->y1;
	}
	nrooms[0]->lenX = nrooms[0]->x2 - nrooms[0]->x1;
	nrooms[0]->lenY = nrooms[0]->y1 - nrooms[0]->y2;
	nrooms[0]->S = nrooms[0]->lenX * nrooms[0]->lenY;
	nrooms[1]->lenX = nrooms[1]->x2 - nrooms[1]->x1;
	nrooms[1]->lenY = nrooms[1]->y1 - nrooms[1]->y2;
	nrooms[1]->S = nrooms[1]->lenX * nrooms[1]->lenY;

	for (int i=0; i< 2; i++) {
		tree_add(parent, nrooms[i]);
	}
	divRoom(parent->right, leaves, map);
	divRoom(parent->left, leaves, map);
}

void div_walls_of_rooms (tree_t*	leaves, map_t* map) {
	UNPACK(field, map);
	if (leaves->parent != NULL) {
		room_t* room = leaves->value;
		room_t* proom;
		if (leaves->parent->right->value == leaves->value) {
			proom = leaves->parent->left->value;
		} else {
			proom = leaves->parent->right->value;
		}
		if (proom->x1 > room->x1) {
			room->doorX[0] = proom->x1;
			room->doorX[1] = proom->x1;
			room->doorY[0] = room->y1 + (proom->y2 - room->y1)/2;
			room->doorY[0] = room->y1 + (proom->y2 - room->y1)/2 - 1;	
		}
		else if (proom->x1 < room->x1) {
			room->doorX[0] = room->x1;
			room->doorX[1] = room->x1;
			room->doorY[0] = room->y1 + (proom->y2 - room->y1)/2;
			room->doorY[0] = room->y1 + (proom->y2 - room->y1)/2 - 1;	
		}
		else if (proom->y1 > room->y1) {
			room->doorX[0] = room->x1 + (proom->x2 - room->x1)/2;
		 	room->doorX[1] = room->x1 + (proom->x2 - room->x1)/2 - 1;		
			room->doorY[0] = proom->y1;
			room->doorY[1] = proom->y1;	
		}
		else if (proom->y1 < room->y2) {
			room->doorX[0] = room->x1 + (proom->x2 - room->x1)/2;
			room->doorX[1] = room->x1 + (proom->x2 - room->x1)/2 - 1;		
			room->doorY[0] = room->y1;
			room->doorY[1] = room->y1;	
		}
		for(int i = 0; i<2; i++) {
			field[room->doorY[i]][room->doorX[i]].symbol = '/' | COLOR_PAIR(3);
	}

	} else {
		return;
	}
	div_walls_of_rooms(leaves->parent, map);
}
