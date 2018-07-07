#pragma once
#include "common.h"
#include "hero.h"

// Structure of coordinates
typedef struct coord{
	int x, y;
} coord_t;

// Structure of vision
typedef struct vision{
	coord_t * view;
} vision_t;

// Map-drawing function
// Renders a chunk of _map bounded by a box;
void draw_map(map_t * _map);

// Function for drawing a field of view
void draw_view(int x, int y, int view_radius, map_t * _map);

//Function for drawing an inventory
void draw_inv(actor_t * actor);

//Function for drawing statistics
void draw_stat(actor_t * actor);

//Function of drawing a actor;
void draw_hero(actor_t * actor);

// Function for drawing features which lay on a terrain
// collect()'s items in a box and draws them in the correct places
void drawFeatures(feature_t * features);

// Function which introduces messages and their history
void draw_text(char * line);

//Function for initialisation
void init_GUI(map_t * _map);

//Main function
//Every function above is called inside render()
void render(map_t * _map, actor_t * actor, feature_t * features);
