#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "caves.h"
#include "common.h"

#define FLAG_PERM  0x1
#define FLAG_TRANS 0x2
#define FLAG_DESTR 0x4

//flags |= FLAG_PERM;
//flags = flags & (~FLAG_PERM);
//flags & FLAG_PERM

map_t * create_map(int height,int width)
{
	map_t *map  = calloc(sizeof(map_t), 1);
	map->buffer = calloc(sizeof(tile_t), width*height);
	map->height = height;
	map->width  = width;	
	return map;
}

void free_map(map_t * map){
	free(map->buffer);
	free(map);
}
map_t * fill_map(map_t * _map, float chance_to_be_alive){
	UNPACK(land, _map); // Defines 2D-array of tile_t named map
	for(int i = 1; i < _map->height-1; i++) {
		for(int j = 1; j < _map->width-1; j++) {
			float chance = rand()/(float) RAND_MAX;
			if(chance < chance_to_be_alive) {
				land[i][j].symbol = '#';
				land[i][j].flags |= FLAG_TRANS;
				}
				else {
				land[i][j].symbol = '.';
				land[i][j].flags |= FLAG_PERM;
				land[i][j].flags |=FLAG_DESTR;
				}
			}
		}
	for(int i = 0; i < _map->width; i++) {
		land[0][i].symbol	             = '#';
		land[_map->height-1][i].symbol = '#';
	}
	for(int i = 1; i < _map->height; i++) {

		land[i][0].symbol	            = '#';
		land[i][_map->width-1].symbol = '#';
	}
	return _map;
}

void simulation_step(map_t * _map, int birth_limit, int death_limit)
{
	UNPACK(land,   _map); // Defines 2D-array of tile_t named map
	map_t * _buffer = create_map(_map->height, _map->width);
	UNPACK(buffer, _buffer);
	for(int i = 1; i < _map->height; i++) {
		for(int j = 1; j < _map->width; j++) {
			int neighbour = 0;
			if(buffer[i+1][j+1].symbol == '#')
				neighbour++;
			if(buffer[i+1][j].symbol   == '#')
				neighbour++;
			if(buffer[i+1][j-1].symbol == '#')
				neighbour++;
			if(buffer[i][j+1].symbol   == '#')
				neighbour++;
			if(buffer[i][j-1].symbol   == '#')
				neighbour++;
			if(buffer[i-1][j+1].symbol == '#')
				neighbour++;
			if(buffer[i-1][j].symbol   == '#')
				neighbour++;
			if(buffer[i-1][j+1].symbol == '#')
				neighbour++;

			if((buffer[i][j].symbol == '#') && (neighbour >= death_limit ))
				land[i][j].symbol = '.';
			else if((buffer[i][j].symbol == '.') && (neighbour >= birth_limit))
				land[i][j].symbol = '#';
		}
	}
	free_map(_buffer);
}

void print_land(map_t * _map)
{
	UNPACK(land, _map); // Defines 2D-array of tile_t named map
	for(int i = 1; i < _map->height-1; i++) {
		for(int j = 1; j < _map->width-1; j++) {
			mvaddch(i, j, land[i][j].symbol);
		}
	}
	refresh();
}
