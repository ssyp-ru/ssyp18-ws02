#include <ncurses.h>
#include "common.h"

#define FLAG_PERM  0x1
#define FLAG_TRANS 0x2
#define FLAG_DESTR 0x4


map_t * create_map(int height,int width);

void free_map(map_t * map);

map_t * fill_map(map_t * _map, float chance_to_be_alive);

void simulation_step(map_t * _map, int birth_limit, int death_limit);

void print_land(map_t * map);
