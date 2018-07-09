#pragma once

#include "rooms.h"

#include <ncurses.h>
#include "common.h"
#include "roomvector.h"
#include "levelvector.h"
#include "kdtree.h"
#include "time.h"

#define FLAG_FEATURE_PERSISTENT 0x00000001
#define FLAG_FEATURE_EDIBILITY  0x00000002
#define FLAG_FEATURE_CLOTHES    0x00000004
#define FLAG_FEATURE_WEAPON     0x00000008

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

// tree-based generation
map_t* mapgen_tree (map_t* map);

//features generation (feature_prototypes)
extern features_vt * prototypes;

void free_protofeatures();

void init_protofeatures();
	
void gen_feature(level_t * level, int num);

void gen_stair(levels_vt * levels, int num);

