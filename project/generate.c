#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include "graphics.h"
#include "generate.h"

map_t* createMap(int width, int height){
	map_t* map = (map_t*) malloc(sizeof(map_t));
	map->cells = (cell_t*)malloc(sizeof(cell_t) * height * width);
	map->height = height;
	map->width = width;
	return map;
}

void clearMap(map_t* mapp){
	UNPACK(map, mapp);
	for (int i = 0; i < mapp->width; i++){
		for (int j = 0; j < mapp->height; j++){
			map[j][i].symbol = ' ';
		}
	}
}

void createDividers(int num, int* data){
	int min = INT32_MAX;
	for (int i = sqrt(num); i > 0; i--){
		if (num % i == 0){
			if (abs(num / i - i) < min){
				min = abs(num / i - i);
				data[0] = i;
				data[1] = num / i;
			}
		}
	}
}

void generateMap(map_t* mapp, int num){
	int z = 0;
	int countx = 0, county = 0;
	int dividers[2];
	createDividers(num, &dividers[0]);
	int cellheight = mapp->height / dividers[0];
	int cellwidth = mapp->width / dividers[1];
	while(z < num){
		if (county >= dividers[1]){
			countx++;
			county = 0;
		}
		int x = countx * cellheight + rand() % cellheight;
 		int y = county * cellwidth + rand() % cellwidth;	
		int cellheightmax = (cellheight - (x - countx * cellheight));
		int cellwidthmax = (cellheight - (x - countx * cellheight));
		int x1 = countx * cellheight + rand() % cellheightmax;
		int y1 = county * cellwidth + rand() % cellwidthmax;
		if (x > x1){
			int swap = x;
			x = x1;
			x1 = swap;
		}
		if (y > y1){
			int swap = y1;
			y1 = y;
			y = swap;
		}
		if (abs(y - y1) < 3){
			continue;
		}
		if (abs(x - x1) < 3){
			continue;	
		}
		rectangle(mapp, x, x1, y, y1);
		county++;
		z++;
	}
}

void generateRoad(map_t* mapp, int num){
	UNPACK(map, mapp);
	int div[2];
	createDividers(num, &div[0]);
	int cellheight = mapp->height / div[0];
	int cellwidth = mapp->width / div[1];
	int x = 0, y = 0, x1 = 0, y1 = 0;
	for (int i = 0; i < div[0]; i++){
		for (int j = 0; j < div[1]; j++){
			while(map[x][y].symbol != '*'){
				x = i * cellheight + rand() % cellheight;
				y = j * cellwidth + rand() % cellwidth;
			}
			char badflag = 'b';
			while(badflag != 'g'){
				int z = rand() % 8;
				switch(z){
					case 0:
						if (i + 1 < div[0]){
							x1 = (i + 1) * cellheight + rand() % cellheight;
							y1 = j * cellwidth + rand() % cellwidth;
							while(map[x1][y1].symbol != '*'){
								x1 = (i + 1) * cellheight + rand() % cellheight;
								y1 = j * cellwidth + rand() % cellwidth;
							}
							badflag = 'g';
						}
					break;
//---------------------------------------------------------
					case 1:
						if (i + 1 < div[0] && j + 1 < div[1]){
							x1 = (i + 1) * cellheight + rand() % cellheight;
							y1 = (j + 1) * cellwidth + rand() % cellwidth;
							while(map[x1][y1].symbol != '*'){
								x1 = (i + 1) * cellheight + rand() % cellheight;
								y1 = (j + 1) * cellwidth + rand() % cellwidth;
							}
							badflag = 'g';
						}
					break;
//---------------------------------------------------------
					case 2:
						if (j + 1 < div[1]){
							x1 = i * cellheight + rand() % cellheight;
							y1 = (j + 1) * cellwidth + rand() % cellwidth;
							while(map[x1][y1].symbol != '*'){
								x1 = i * cellheight + rand() % cellheight;
								y1 = (j + 1) * cellwidth + rand() % cellwidth;
							}
							badflag = 'g';
						}
//---------------------------------------------------------
					break;
					case 3:
						if (i - 1 >= 0 && j + 1 < div[1]){
							x1 = (i - 1) * cellheight + rand() % cellheight;
							y1 = (j + 1) * cellwidth + rand() % cellwidth;
							while(map[x1][y1].symbol != '*'){
								x1 = (i - 1) * cellheight + rand() % cellheight;
								y1 = (j + 1) * cellwidth + rand() % cellwidth;
							}
							badflag = 'g';
						}
//---------------------------------------------------------
					break;
					case 4:
						if (i - 1 >= 0){
							x1 = (i - 1) * cellheight + rand() % cellheight;
							y1 = j * cellwidth + rand() % cellwidth;
							while(map[x1][y1].symbol != '*'){
								x1 = (i - 1) * cellheight + rand() % cellheight;
								y1 = j * cellwidth + rand() % cellwidth;
							}
							badflag = 'g';
						}
					break;
//---------------------------------------------------------
					case 5:
						if (i - 1 >= 0 && j - 1 >= 0){
							x1 = (i - 1) * cellheight + rand() % cellheight;
							y1 = (j - 1) * cellwidth + rand() % cellwidth;
							while(map[x1][y1].symbol != '*'){
								x1 = (i - 1) * cellheight + rand() % cellheight;
								y1 = (j - 1) * cellwidth + rand() % cellwidth;
							}
							badflag = 'g';
						}
					break;
//---------------------------------------------------------
					case 6:
						if (j - 1 >= 0){
							x1 = i * cellheight + rand() % cellheight;
							y1 = (j - 1) * cellwidth + rand() % cellwidth;
							while(map[x1][y1].symbol != '*'){
								x1 = i * cellheight + rand() % cellheight;
								y1 = (j - 1) * cellwidth + rand() % cellwidth;
							}
							badflag = 'g';
						}
					break;
//---------------------------------------------------------
					case 7:
						if (i + 1 < div[0] && j - 1 >= 0){
							x1 = (i + 1) * cellheight + rand() % cellheight;
							y1 = (j - 1) * cellwidth + rand() % cellwidth;
							while(map[x1][y1].symbol != '*'){
								x1 = (i + 1) * cellheight + rand() % cellheight;
								y1 = (j - 1) * cellwidth + rand() % cellwidth;
							}
							badflag = 'g';
						}
					break;
				}
			}
			if (x > x1){
				int swap = x;
				x = x1;
				x1 = swap;
			}
			if (y > y1){
				int swap = y1;
				y1 = y;
				y = swap;
			}
			drawSimpleLine(mapp, x, x1, y, y1);
		}
	}
}
