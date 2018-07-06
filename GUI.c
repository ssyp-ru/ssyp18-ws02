#include "GUI.h"
#include <ncurses.h>

struct GUI {
	WINDOW * mapField;
	WINDOW * statField;
	WINDOW * invField;
	WINDOW * mesField;
} GUI;
void drawMap(map_t * _map){
	int sizeX = _map->width;
	int sizeY = _map->height;
	UNPACK(map, _map);
	for(int i = 0; i < sizeX; i++){
		for(int k = 0; k < sizeY; k++){
			mvwaddch(GUI.mapField, k, i, map[k][i].symbol);
		}
	}
}

void drawView(int xPos, int yPos, int viewRadius, map_t * _map){
	UNPACK(map, _map);
	int index = 0;
	float angle = 0;
	vision_t * vision = calloc(1, sizeof(vision_t));
	vision->view = calloc(10000, sizeof(coord_t));
	for(angle = -viewRadius; angle <= viewRadius; angle += 0.2){
		for(float x = 0; x >= -viewRadius; x -= 0.2){	
			int y1 = angle * x + yPos - 0.5;
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
		for(float x = 0; x <= viewRadius; x += 0.2){
			int y1 = angle * x + yPos - 0.5;
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
			mvwaddch(GUI.mapField, y, x, '.');
		}	
}
void drawBorder(WINDOW * window){
	int x,y;
	getmaxyx(window, y, x);
	for(int i = 0; i < y; i++){
		if(i == 0 || i == y - 1){
			mvwaddch(window, i, 0, '+');
			mvwaddch(window, i, x - 1, '+');
		}
		else{
			mvwaddch(window, i, 0, '|');
			mvwaddch(window, i, x - 1, '|');
		}
	}
	for(int i = 0; i < x; i++){
		if(i == 0 || i == x - 1){
			mvwaddch(window, 0, i, '+');
			mvwaddch(window, y - 1, i, '+');
		}
		else{
			mvwaddch(window, 0, i, '-');
			mvwaddch(window, y - 1, i, '-');
		}
	}
}
void drawText(char * line){
	mvwprintw(GUI.mesField, 2, 2, line);
}

/*void drawFeatures(kdtree_t * features, WINDOW * window){
	...
}*/

void drawInv(hero_t * hero){
	 int amount	= hero->inventory.amount;
	 for(int i = 0; i < amount; i++){
			mvwprintw(GUI.invField, i * 2 + 2, 2,
				 	hero->inventory.item[i].description);
	 }
}

void drawStats(hero_t * hero){
	int hp = hero->hp;
	int strength = hero->strength;
	int agility = hero->agility;
	int stamina = hero->stamina;
	mvwprintw(GUI.statField, 2, 2, "HP: %d", hp);
	mvwprintw(GUI.statField, 4, 2, "Strength: %d", strength);
	mvwprintw(GUI.statField, 6, 2, "Agility: %d", agility);
	mvwprintw(GUI.statField, 8, 2, "Stamina: %d", stamina);
}

void drawHero(hero_t * hero){
	int xPos = hero->xPos;
	int yPos = hero->yPos;
	mvwaddch(GUI.mapField, yPos, xPos, hero->look);	
}

void closeWindows(){
	delwin(GUI.mapField);
	delwin(GUI.mesField);
	delwin(GUI.invField);
	delwin(GUI.statField);

}

void render(map_t * _map, hero_t * hero){
	int xPos = hero->xPos;
	int yPos = hero->yPos;
	drawMap(_map);
	drawView(xPos, yPos, 5, _map);
	drawInv(hero);
	drawText("There is nothing here!");
	drawStats(hero);
	drawHero(hero);
	//drawFeatures(features, window);
	drawBorder(GUI.mapField);
	drawBorder(GUI.mesField);
	drawBorder(GUI.invField);
	drawBorder(GUI.statField);
	wrefresh(GUI.mapField);
	wrefresh(GUI.mesField);
	wrefresh(GUI.invField);
	wrefresh(GUI.statField);
}

void initGUI(map_t * _map){
	int sizeX = _map->width;
	int sizeY = _map->height;
	int step = 40;
	int yText = 6;
	int yInv = 20;
	GUI.mapField = newwin(sizeY, sizeX, 0, step);
	GUI.mesField = newwin(yText, step, 0, 0);
	GUI.invField = newwin(yInv,step, yText, 0);
	GUI.statField = newwin(sizeY - yText - yInv, step, yText + yInv, 0);
}

