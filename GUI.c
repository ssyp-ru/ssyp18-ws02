#include "GUI.h"
#include <ncurses.h>
#include <stdlib.h>
#include "common.h"

struct GUI {
	WINDOW * map_field;
	WINDOW * stat_field;
	WINDOW * inv_field;
	WINDOW * mes_field;
} GUI;

void draw_map(map_t * _map, box_t box) {
	int size_x = box.width;
	int size_y = box.height;
	UNPACK(map, _map);
	if(size_x > _map->width)
		size_x = _map->width;
	if(size_y > _map->height)
		size_y = _map->height;
	for(int k = 1; k < size_y - 1; k++){
		for(int i = 1; i < size_x - 1; i++){
			if(k + box.y < _map->height && i + box.x < _map->width)
				mvwaddch(GUI.map_field, k, i, map[k + box.y][i + box.x].symbol);
		}
	}
#ifdef DEBUG
  char buffer[100];
  snprintf(buffer, 100, " Box: (%d:%d, %d:%d)\n", box.x, box.x+box.width,
                                                 box.y, box.y+box.height);
	mvwprintw(GUI.mes_field, 1, 1, buffer);
#endif /* DEBUG */
}

void draw_view(int x2, int y2, int view_radius,
	 	map_t * _map, box_t box){
  //TODO: Split this into view calculator and
  // view renderer.
	UNPACK(map, _map);
	int index = 0;
	float angle = 0;
	vision_t * vision = calloc(1, sizeof(vision_t));
	vision->view = calloc(10000, sizeof(coord_t));
	for(angle = -view_radius; angle <= view_radius; angle += 0.2){
		for(float x = 0; x >= -view_radius; x -= 0.2){	
			int y1 = angle * x + y2;
			int x1 = x + x2 + 0.5;
			int rad_x = x1 - x2;
			int rad_y = y1 - y2;
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
			int y1 = angle * x + y2;
			int x1 = x + x2 + 0.5;
			int rad_x = x1 - x2;
			int rad_y = y1 - y2;
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
		int y1 = y + y2;
		int x1 = x2;
		if(map[y1][x1].flags & FLAG_SOLID){
			break;
		}else{
			vision->view[index].y = y1;
			vision->view[index].x = x1;
			index++;
		}
	}	
	for(int y = 0; y <= view_radius; y++){
		int y1 = y + y2;
		int x1 = x2;
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
			mvwaddch(GUI.map_field, y - box.y, x - box.x, '.');
		}	
}

void draw_borders(WINDOW * window){
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
	mvwprintw(GUI.mes_field, 2, 2, "                           ");
	mvwprintw(GUI.mes_field, 2, 2, line);
}

void draw_features(feature_t * features){	
  // FIXME: Use kdtree_t;
	//	mvwaddch(GUI.map_field, features[i].y, features[i].x, features[i].symbol);	
}

void draw_inv(actor_t * actor){
	 int inv_length = actor->inventory->length;
	 for(int i = 0; i < inv_length; i++){
		 if(actor->inventory->data[i].flags & FLAG_STACKABLE){
			mvwprintw(GUI.inv_field, i * 2 + 2, 2, "%d. %s: %d", i,
				 	actor->inventory->data[i].description,
					actor->inventory->data[i].amount);
		 }else{
				 mvwprintw(GUI.inv_field, i * 2 + 2, 2, "%d. %s", i,
				 	actor->inventory->data[i].description);
			}
	 }
}

void draw_stats(actor_t * hero) {
	int hp = hero->hp;
	int strength = hero->strength;
	int agility = hero->agility;
	int stamina = hero->stamina;
	mvwprintw(GUI.stat_field, 2, 2, "HP: %d", hp);
	mvwprintw(GUI.stat_field, 4, 2, "Strength: %d", strength);
	mvwprintw(GUI.stat_field, 6, 2, "Agility: %d", agility);
	mvwprintw(GUI.stat_field, 8, 2, "Stamina: %d", stamina);
}

void draw_actor(actor_t * actor, box_t box) {
  int x = actor->x;
  int y = actor->y;
  mvwaddch(GUI.map_field, y - box.y, x - box.x, actor->symbol);
}

void close_windows() {
	delwin(GUI.map_field);
	delwin(GUI.mes_field);
	delwin(GUI.inv_field);
	delwin(GUI.stat_field);

}

void render(actor_t   * hero,
            actors_vt   * actors,
	 	        msgs_t    * msgs) {
  int w = 0, h = 0;
  level_t * level = hero->level;
  map_t * map = level->map;
  getmaxyx(GUI.map_field, h, w);
  h -= 2;
  w -= 2;
  box_t box = {.x = hero->x - w/2,
               .y = hero->y - h/2,
               .width = w,
               .height = h};
  if (box.x < 0)
    box.x = 0;
  if(box.x+w > map->width)
    box.x = map->width - w;
  if (box.y < 0)
    box.y = 0;
  if(box.y+h > map->height)
    box.y = map->height - h;

  //FIXME: Replace this with kdtree
  feature_t * features = level->features;
	
  draw_map(map, box);
	draw_view(hero->x, hero->y, 4, map, box);
	draw_inv(hero);
	draw_text(msgs->buffer[msgs->cur].line);
	draw_stats(hero);
	draw_features(features);
	draw_actor(hero, box);

  for(int i = 0; i < actors->length; i++)
    draw_actor(actors->data[i], box);

	draw_borders(GUI.map_field);
	draw_borders(GUI.mes_field);
	draw_borders(GUI.inv_field);
	draw_borders(GUI.stat_field);

	wrefresh(GUI.map_field);
	wrefresh(GUI.mes_field);
	wrefresh(GUI.inv_field);
	wrefresh(GUI.stat_field);
}

void init_GUI() {
	int screen_width = 0;
	int screen_height = 0;
  getmaxyx(stdscr, screen_height, screen_width);
  const int HUD_width = 30; 
	const int text_height = 6;
	const int stats_height = 8;
	GUI.map_field = newwin(screen_height,
                         screen_width-HUD_width,
                         0, HUD_width);
	GUI.mes_field = newwin(text_height, HUD_width,
                         0, 0);
	GUI.inv_field = newwin(screen_height - text_height-stats_height, HUD_width,
                         text_height, 0);
	GUI.stat_field = newwin(stats_height, HUD_width,
                          screen_height - stats_height, 0);
}

