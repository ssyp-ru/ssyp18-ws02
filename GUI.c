#include "GUI.h"
#include <ncurses.h>

struct GUI {
	WINDOW * map_field;
	WINDOW * stat_field;
	WINDOW * inv_field;
	WINDOW * mes_field;
} GUI;
void draw_map(map_t * _map){
	int size_x = _map->width;
	int size_y = _map->height;
	UNPACK(map, _map);
	for(int i = 0; i < size_x; i++){
		for(int k = 0; k < size_y; k++){
			mvwaddch(GUI.map_field, k, i, map[k][i].symbol);
		}
	}
}

void draw_view(int x, int y, int view_radius, map_t * _map){
	UNPACK(map, _map);
	int index = 0;
	float angle = 0;
	vision_t * vision = calloc(1, sizeof(vision_t));
	vision->view = calloc(10000, sizeof(coord_t));
	for(angle = -view_radius; angle <= view_radius; angle += 0.2){
		for(float x = 0; x >= -view_radius; x -= 0.2){	
			int y1 = angle * x + y - 0.5;
			int x1 = x + x + 0.5;
			int rad_x = x1 - x;
			int rad_y = y1 - y;
			if(rad_x * rad_x + rad_y * rad_y <= view_radius * view_radius){
				if(map[y1][x1].flags & FLAG_SOLID){
						break;
				}else{
					vision->view[index].y = y1;
					vision->view[index].x = x1;
					index++;
				}
			}			
		}
		for(float x = 0; x <= view_radius; x += 0.2){
			int y1 = angle * x + y - 0.5;
			int x1 = x + x + 0.5;
			int rad_x = x1 - x;
			int rad_y = y1 - y;
			if(rad_x * rad_x + rad_y * rad_y <= view_radius * view_radius){
				if(map[y1][x1].flags & FLAG_SOLID){
						break;
				}else{
					vision->view[index].y = y1;
					vision->view[index].x = x1;
					index++;	
				}
			}
		}
	}				
	for(int y = 0; y >= -view_radius; y--){
		int y1 = y + y;
		int x1 = x;
		if(map[y1][x1].flags & FLAG_SOLID){
			break;
		}else{
			vision->view[index].y = y1;
			vision->view[index].x = x1;
			index++;
		}
	}	
	for(int y = 0; y <= view_radius; y++){
		int y1 = y + y;
		int x1 = x;
		if(map[y1][x1].flags & FLAG_SOLID){
			break;
		}else{
			vision->view[index].y = y1;
			vision->view[index].x = x1;
			index++;
		}
	}	
	for(int i = 0; i < index; i++){
			int x = vision->view[i].x;
			int y = vision->view[i].y;
			mvwaddch(GUI.map_field, y, x, '.');
		}	
}
void draw_borded(WINDOW * window){
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
void draw_text(char * line){
	mvwprintw(GUI.mes_field, 2, 2, line);
}

/*void drawFeatures(kdtree_t * features, WINDOW * window){
	...
}*/

void draw_inv(hero_t * hero){
	 int amount	= hero->inventory.amount;
	 for(int i = 0; i < amount; i++){
			mvwprintw(GUI.inv_field, i * 2 + 2, 2,
				 	hero->inventory.item[i].description);
	 }
}

void draw_stats(hero_t * hero){
	int hp = hero->hp;
	int strength = hero->strength;
	int agility = hero->agility;
	int stamina = hero->stamina;
	mvwprintw(GUI.stat_field, 2, 2, "HP: %d", hp);
	mvwprintw(GUI.stat_field, 4, 2, "Strength: %d", strength);
	mvwprintw(GUI.stat_field, 6, 2, "Agility: %d", agility);
	mvwprintw(GUI.stat_field, 8, 2, "Stamina: %d", stamina);
}

void draw_hero(hero_t * hero){
	int x = hero->x;
	int y = hero->y;
	mvwaddch(GUI.map_field, y, x, hero->look);	
}

void closeWindows(){
	delwin(GUI.map_field);
	delwin(GUI.mes_field);
	delwin(GUI.inv_field);
	delwin(GUI.stat_field);

}

void render(map_t * _map, hero_t * hero){
	int x = hero->x;
	int y = hero->y;
	draw_map(_map);
	draw_view(x, y, 5, _map);
	draw_inv(hero);
	draw_text("There is nothing here!");
	draw_stats(hero);
	draw_hero(hero);
	//drawFeatures(features, window);
	draw_borded(GUI.map_field);
	draw_borded(GUI.mes_field);
	draw_borded(GUI.inv_field);
	draw_borded(GUI.stat_field);
	wrefresh(GUI.map_field);
	wrefresh(GUI.mes_field);
	wrefresh(GUI.inv_field);
	wrefresh(GUI.stat_field);
}

void init_GUI(map_t * _map){
	int size_x = _map->width;
	int size_y = _map->height;
	int step = 40;
	int yText = 6;
	int yInv = 20;
	GUI.map_field = newwin(size_y, size_x, 0, step);
	GUI.mes_field = newwin(yText, step, 0, 0);
	GUI.inv_field = newwin(yInv,step, yText, 0);
	GUI.stat_field = newwin(size_y - yText - yInv, step, yText + yInv, 0);
}

