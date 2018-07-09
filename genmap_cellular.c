#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "genmap.h"
#include "common.h"
//using flags: FLAG_SOLID 0x1
//             FLAG_TRANS 0x2
//             FLAG_DESTR 0x4


map_t * mapgen_cellular(map_t * _map,
                        float chance_to_be_alive) {
	srand(time(NULL));
	UNPACK(land, _map); // Defines 2D-array of tile_t named map
	for(int i = 1; i < _map->height-1; i++) {
		for(int j = 1; j < _map->width-1; j++) {
			float chance = rand()/(float) RAND_MAX;
			if(chance < chance_to_be_alive) {
				land[i][j].symbol = '#';
				land[i][j].flags |= FLAG_TRANS;
				land[i][j].flags |= FLAG_SOLID;
			} else {
				land[i][j].symbol = '.';
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
		land[i][0].flags             |= FLAG_TRANS;
		land[i][_map->width-1].symbol = '#';
		land[i][_map->width-1].flags |= FLAG_TRANS;
	}
	return _map;
}

map_t * simulation_step_cellular(map_t * _map,
                                 int birth_limit, int death_limit, int num_of_steps) {
	UNPACK(land,
	       _map); // Defines 2D-array of tile_t named map
	map_t * _buffer = create_map(_map->height, _map->width);
	UNPACK(buffer, _buffer);
	memcpy(_buffer->buffer, _map->buffer,
	       sizeof(tile_t)*_map->height*_map->width);
	for(int k = 0; k < num_of_steps; k++) {
		for(int i = 1; i < _map->height-1; i++) {
			for(int j = 1; j < _map->width-1; j++) {
				int neighbour = 0;
				if(buffer[i+1][j+1].flags & FLAG_SOLID)
					neighbour++;
				if(buffer[i+1][j].flags   & FLAG_SOLID)
					neighbour++;
				if(buffer[i+1][j-1].flags & FLAG_SOLID)
					neighbour++;
				if(buffer[i][j+1].flags   & FLAG_SOLID)
					neighbour++;
				if(buffer[i][j-1].flags   & FLAG_SOLID)
					neighbour++;
				if(buffer[i-1][j+1].flags & FLAG_SOLID)
					neighbour++;
				if(buffer[i-1][j].flags   & FLAG_SOLID)
					neighbour++;
				if(buffer[i-1][j+1].flags & FLAG_SOLID)
					neighbour++;

				if         ((buffer[i][j].flags & FLAG_SOLID)
				            && (neighbour >= death_limit)) {
					land[i][j].symbol = '.';
					land[i][j].flags ^= FLAG_SOLID;
					land[i][j].flags ^= FLAG_TRANS;
					land[i][j].flags |= FLAG_DESTR;
				} else if((!(buffer[i][j].flags & FLAG_SOLID))
				          && (neighbour >= birth_limit)) {
					land[i][j].symbol = '#';
					land[i][j].flags |= FLAG_SOLID;
					land[i][j].flags |= FLAG_TRANS;
					land[i][j].flags ^= FLAG_DESTR;
				}
			}
		}
	}

	free_map(_buffer);
	return _map;
}
