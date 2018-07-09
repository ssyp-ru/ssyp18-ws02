#include "networking.h"
//void map_update(map_t* _map, update_map_t* msg);






update_map_t * map_serialize(map_t* _map, int y, int x, int height, int width){
	int width = _map->width;
	int height = _map->height;
	update_map_t * update_map = calloc(1, sizeof(update_map_t));
	int size = 0;
	UNPACK(map, _map);
	for(int i = y; i < y + height; i++){
		for(int k = x; k < x + width; k++){
			update_map->s[size] = map[i][k];			
			size++;
		}
	}
	return update_map;
}

void client_map(map_t * map, update_map_t * chunk){
		UNPACK(map, _map);
		int y = chunk->y;
		int x = chunk->x;
		int width = chunk->width;
		int height = chunk->height;
		int size = 0;
		for(int i = y; i < y + height; i++){
			for(int k = x; k < x + width; k++){
				map[i][k] = chunk->s[size];
				size++;
			}
		}
}
