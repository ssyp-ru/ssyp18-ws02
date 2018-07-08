#include "GUI.h"
#include <ncurses.h>

void drawMap(map_t * _map, WINDOW * window){
	int sizeX = _map->width;
	int sizeY = _map->height;
	UNPACK(map, _map);
	for(int i = 0; i < sizeX; i++){
		for(int k = 0; k < sizeY; k++){
			mvaddch(window k, i + STEP, map[k][i].s);
		}
	}
}

void drawView(int xPos, int yPos, int viewRadius, map_t * _map){
	UNPACK(map, _map);
	int index = 0;
	float angle = 0;
	vision_t * vision = calloc(1, sizeof(vision_t));
	vision->view = calloc(10000, sizeof(coord_t));
	for(angle = -4; angle <= 4; angle += 0.2){
		for(float x = 0; x >= -4; x -= 0.2){	
			int x1 = x + xPos + 0.5;
			int y1 = angle * x + yPos;
			int radX = x1 - xPos;
			int radY = y1 - yPos;
			if(radX * radX + radY * radY <= viewRadius * viewRadius){
				if(map[y1][x1].flags & FLAG_SOLID){
						break;
				}else{
					vision->view[index].yPos = y1;
					vision->view[index].xPos = x1;
					index++;
				}
			}			
		}
		for(float x = 0; x <= 4; x += 0.2){
			int y1 = angle * x + yPos;
			int x1 = x + xPos + 0.5;
			int radX = x1 - xPos;
			int radY = y1 - yPos;
			if(radX * radX + radY * radY <= viewRadius * viewRadius){
				if(map[y1][x1].flags & FLAG_SOLID){
						break;
				}else{
					vision->view[index].yPos = y1;
					vision->view[index].xPos = x1;
					index++;	
				}
			}
		}
	}				
	for(int y = 0; y >= -viewRadius; y--){
		int y1 = y + yPos;
		int x1 = xPos;
		if(map[y1][x1].flags & FLAG_SOLID){
			break;
		}else{
			vision->view[index].yPos = y1;
			vision->view[index].xPos = x1;
			index++;
		}
	}	
	for(int y = 0; y <= viewRadius; y++){
		int y1 = y + yPos;
		int x1 = xPos;
		if(map[y1][x1].flags & FLAG_SOLID){
			break;
		}else{
			vision->view[index].yPos = y1;
			vision->view[index].xPos = x1;
			index++;
		}
	}	
	for(int i = 0; i < index; i++){
			int x = vision->view[i].xPos;
			int y = vision->view[i].yPos;
			mvaddch(window y, x + STEP, '.');
		}	
}

void render(int xPos, int yPos, map_t * _map, WINDOW * window){
	drawMap(_map, window);
	drawView(xPos, yPos, 4, _map);
}

void initGUI(map_t * _map){
	WINDOW * window;
	int sizeX = _map->width;
	int sizeY = _map->height;
	window = newwin(sizeY, sizeX, 0, 20);
}

