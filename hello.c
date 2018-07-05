#include <ncurses.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include "hero.h"
#define COLOR_GREY COLOR_MAGENTA
#define COLOR_BROWN COLOR_CYAN
#define STEP 20

map_t * genLab(map_t *_map){
	int sizeX = _map->width;
	int sizeY = _map->height;
	UNPACK(map, _map);
	int originX, originY, index = 1; 
	int rand4, countCells = 1500;
	for(int i = 0; i < sizeX; i++){
		for(int k = 0; k < sizeY; k++){
			map[k][i].flags |= FLAG_SOLID;
		}
	}
	originX = rand() % (sizeX - 3) + 1;
	originY = rand() % (sizeY - 3) + 1;
	map[originY][originX].flags &= ~FLAG_SOLID;
	while(index < countCells){
		rand4 = rand() % 4;
		int counter = 0;
		for(int i = -1; i <= 1; i++){
			for(int k = -1; k <= 1; k++){
				if(k != 0 || i != 0)
					counter += !(map[originY + k][originX + i].flags & FLAG_SOLID);
			}
		}
		if(map[originY][originX].flags & FLAG_SOLID && counter < 4){
			map[originY][originX].flags &= ~FLAG_SOLID;
			index++;
		}
		else{
			originX = rand() % (sizeX - 3) + 1;
			originY = rand() % (sizeY - 3) + 1;
		}
		switch(rand4){
			case 0:	
				if(originY - 1 > 0)
					originY--;
				break;
			case 1:
				if(originY + 2 < sizeY)
					originY++;
				break;
			case 2:
				if(originX - 1 > 0)
					originX--;
				break;
			case 3:
				if(originX + 2 < sizeX)
					originX++;
				break;	
		}
	}
	return _map;
}
map_t * genTestRoom(map_t *_map, int xPos, int yPos){
	UNPACK(map, _map);
	for(int i1 = 0; i1 < 10; i1++){
		for(int k1 = 0; k1 < 10; k1++){
			map[i1 - 5 + yPos][k1 - 5 + xPos].flags &= ~FLAG_SOLID;
			map[i1 - 5 + yPos][k1 - 5 + xPos].s = ' ';
		}
	}
	return _map;
}
map_t * genRooms(map_t *_map){
	int sizeX = _map->width;
	int sizeY = _map->height;
	UNPACK(map, _map);
	for(int i = 0; i < 5; i++){
		int randX = rand() % (sizeX - 15) + 2;
		int randY = rand() % (sizeY - 15) + 2;
		int siz1 = rand() % 3 + 6;
		int siz2 = rand() % 3 + 6;
		int counter = 0;
		for(int k = randY; k <= randY + siz1; k++){
			for(int l = randX; l <= randX + siz2; l++){
				if(k == randY || k == randY + siz1
					|| l == randX || l == randX + siz2){
					if(!(map[k - 1][l].flags & FLAG_SOLID) &&
					!(map[k + 1][l].flags & FLAG_SOLID) && counter < 3){
						//Place for a door
						map[k][l].flags &= ~FLAG_SOLID;
						counter++;
					}
					else if(!(map[k][l - 1].flags & FLAG_SOLID) &&
					!(map[k][l + 1].flags & FLAG_SOLID) && counter < 3){
						//Place for a door
						map[k][l].flags &= ~FLAG_SOLID;
						counter++;	
					}
					else
						map[k][l].flags |= FLAG_SOLID;
				}
				else map[k][l].flags &= ~FLAG_SOLID;		
			}
		}
		counter = 0;
	}
	return _map;
}
void initColors(){
	init_color(COLOR_GREY,300, 300, 300);	
	init_color(COLOR_BROWN,200, 100, 0);
	init_pair(1, COLOR_RED, COLOR_BLACK);	
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3,COLOR_BLUE, COLOR_BLACK);
	init_pair(4,COLOR_GREEN, COLOR_BLACK);
	init_pair(5,COLOR_BLUE, COLOR_BLACK);
	init_pair(6,COLOR_GREY, COLOR_BLACK);
	init_pair(7,COLOR_BROWN, COLOR_BLACK);
}
void initScr(){
	initscr();
	start_color();
	keypad(stdscr,true);
	noecho();
	halfdelay(100);
	curs_set(0);
}
void drawView(int index, map_t *_map, hero_t *_hero){
	UNPACK(map, _map);
	for(int i = 0; i < index; i++){
		int x = _hero->view[index].xPos;
		int y = _hero->view[index].yPos;
		if(!(map[y][x].flags & FLAG_SOLID)){
			mvaddch(y, x + STEP, '.');
		}
	}
}
int checkView(int viewRadius, map_t *_map, hero_t *_hero){
	UNPACK(map, _map);
	int index = 0;
	float angle = 0;
	int xPos = _hero->xPos;
	int yPos = _hero->yPos;	
	for(angle = -4; angle <= 4; angle += 0.2){
		for(float x = 0; x >= -4; x -= 0.2){	
			if(map[(int)(angle * x + yPos)]
				[(int)(x + xPos + 0.5)].flags & FLAG_SOLID){
					break;
			}else{
				_hero->view[index].yPos = (int)(angle * x + yPos);
				_hero->view[index].xPos = (int)(x + xPos + 0.5);
				index++;
			}					
		}
		for(float x = 0; x <= 4; x += 0.2){	
			if(map[(int)(angle * x + yPos)]
				[(int)(x + xPos + 0.5)].flags & FLAG_SOLID){
					break;
			}else{
				_hero->view[index].yPos = (int)(angle * x + yPos);
				_hero->view[index].xPos = (int)(x + xPos + 0.5);
				index++;	
			}
		}
	}				
	for(int y = 0; y >= -viewRadius; y--){
		if(map[(int)(y + yPos)][xPos].flags & FLAG_SOLID){
			break;
		}else{
			_hero->view[index].yPos = (int)(y + yPos);
			_hero->view[index].xPos = xPos;
			index++;
		}
	}	
	for(int y = 0; y <= viewRadius; y++){
		if(map[(int)(y + yPos)][xPos].flags & FLAG_SOLID){
			break;
		}else{
			_hero->view[index].yPos = (int)(y + yPos);
			_hero->view[index].xPos = xPos;
			index++;
		}
	}
	return index;
}
map_t * initMap(int sizeX, int sizeY){
	map_t *_map = calloc(1, sizeof(map_t));
	_map->buffer = calloc(sizeX * sizeY, sizeof(tile_t));
	_map->width = sizeX;
	_map->height = sizeY;
	return _map;
}
hero_t * initHero(int xPos, int yPos, int inventorySize, int maxView){
	hero_t * _hero = calloc(1, sizeof(hero_t));
	_hero->view = calloc(maxView * maxView, sizeof(vision_t));
	_hero->inventory = calloc(inventorySize, sizeof(inventory_t));
	_hero->hp = 2;
	_hero->xPos = xPos;
	_hero->yPos = yPos;
	_hero->look = '@';
	return _hero;
}
void drawMap(map_t *_map){
	int sizeX = _map->width;
	int sizeY = _map->height;
	UNPACK(map, _map);
	for(int i = 0; i < sizeX; i++){
		for(int k = 0; k < sizeY; k++){
			mvaddch(k, i + STEP, map[k][i].s);
		}
	}
}
void behave(hero_t * _hero, map_t * _map){
	UNPACK(map, _map);
	int x = _hero->xPos;
	int y = _hero->yPos;
	int moveX = 0; 
	int moveY = 0;
	switch(getch()){
		case KEY_UP:
			moveY = -1;
			break;
		case KEY_DOWN:
			moveY = 1;
			break;
		case KEY_RIGHT:
			moveX = 1;
			break;
		case KEY_LEFT:
			moveX = -1;
			break;
	}
	if(!(map[y + moveY][x + moveX].flags & FLAG_SOLID)){
		_hero->xPos += moveX;
		_hero->yPos += moveY;
	}
}
void drawText(map_t * _map, int xPos, int yPos){
	char line[50] = "";
	mvprintw(1,1,"             ");
	UNPACK(map, _map);
	switch(map[yPos][xPos].s){
		case ' ':
			strcpy(line, "_");
			break;
		case 'P':
			strcpy(line, "A key!");
			break;
		case '$':
			strcpy(line, "Some money!");
			break;
		case 'Y':
			strcpy(line, "A slingshot!");
			break;
		case '*':
			strcpy(line, "A bullet!");
			break;
	}
	mvprintw(1, 1, "%s", line);
}
map_t * makeWalls(map_t * _map){
	int sizeX = _map->width;
	int sizeY = _map->height;
	UNPACK(map, _map);
	for(int i = 0; i < sizeX; i++){
		for(int k = 0; k < sizeY; k++){	
			if(map[k][i].flags & FLAG_SOLID){
				if(k == 0 || k == sizeY - 1 ||
					i == 0 || i == sizeX - 1){
					map[k][i].s = '#' | COLOR_PAIR(1);
				}else{
					if(!(map[k - 1][i].flags & FLAG_SOLID) &&
						!(map[k + 1][i].flags & FLAG_SOLID))
						map[k][i].s = '-' | COLOR_PAIR(6);
					else
						map[k][i].s = '|' | COLOR_PAIR(6);
					if(map[k - 1][i].flags & FLAG_SOLID
						&& map[k + 1][i].flags & FLAG_SOLID
						&& map[k][i - 1].flags & FLAG_SOLID
						&& map[k][i + 1].flags & FLAG_SOLID)
						map[k][i].s = '#' | COLOR_PAIR(6);
				}
			}else
				map[k][i].s = ' ';
		}
	}
	return _map;
}
//Function of main act
void act(){
	//Initialisation of tools for program
	int sizeX = 0;
	int sizeY = 0, viewRadius = 4;
	int xPos , step = 20;
	int yPos;
	//Initialisation of colors
	initColors();
	srand(time(NULL));
	getmaxyx(stdscr,sizeY,sizeX);
	xPos = 40;
	yPos = 20;
	sizeX -= step;
	//Creating map and unpacking it
	map_t * _map = initMap(sizeX, sizeY);
	//Creating main character, his vision and  an inventory
	hero_t *_hero = initHero(xPos, yPos, 25, 100);
	//Generation of labirint with rooms
	_map = genLab(_map);
	_map = genRooms(_map);
	//Introduce some walls and a testing room
	_map = makeWalls(_map);	
	_map = genTestRoom(_map, xPos, yPos);
	bool ex = false;
	//Interactive part
	while(!ex){   
		//Text messages	
		drawText(_map, _hero->xPos, _hero->yPos);
		//Output of a map
		drawMap(_map);
		//Field of view
		drawView(checkView(viewRadius, _map, _hero), _map, _hero);        
		//Draw a hero ( not a function until future additions )
		mvaddch(_hero->yPos, _hero->xPos + STEP, _hero->look);
		//Input and act
		behave(_hero, _map);
		refresh();
		clear();
	}
	getch();
	endwin();		
} 
//Main function
int main(int argc, char*argv[]){
	initScr();
	act();
}
