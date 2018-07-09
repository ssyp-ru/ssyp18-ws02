#pragma once

#include <ncurses.h>
#include "common.h"
#include "roomvector.h"

#define FLAG_PERM  0x1
#define FLAG_TRANS 0x2
#define FLAG_DESTR 0x4

#define COLOR_GREY COLOR_MAGENTA
#define COLOR_BROWN COLOR_CYAN

//general functions
map_t * create_map(int height,int width);

void print_map(map_t * map);

void free_map(map_t * map);

//generation using cellural automaton (cellural_map.c)
map_t * mapgen_cellular(map_t * _map,
                        float chance_to_be_alive);

map_t * simulation_step_cellular(map_t * _map,
                                 int birth_limit, int death_limit, int num_of_steps);

//generation using shrews algorithm (shrew_map.c)
map_t * mapgen_shrew(map_t *_map);

// What the hell does it do?
map_t * mapgen_rooms_shrew(map_t *_map);
map_t * make_walls_shrew(map_t * _map);

//generation using recursive devision (recursive_devision_map.c)
map_t* mapgen_recdev(map_t * map);

