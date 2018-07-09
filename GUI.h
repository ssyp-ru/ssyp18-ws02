#pragma once
#include "common.h"
#include "actor.h"
#include "kdtree.h"

// Structure of coordinates
typedef struct coord{
	int x, y;
} coord_t;

// Structure of vision
typedef struct vision{
	coord_t * view;
} vision_t;

typedef struct message{
	char * line;
	int length;
} msg_t;

typedef struct messages{
	msg_t * buffer;
	int size;
	int max_size;
	int cur;
}	msgs_t;

// Map-drawing function
// Renders a chunk of _map bounded by a box;
void draw_map(map_t * _map, box_t box);

// Function for drawing a field of view
void calculate_view(int x, int y,
               int view_radius,
	 	           map_t * _map,
               box_t box);

//Function for drawing an inventory
void draw_inv(actor_t * actor);

//Function for drawing statistics
void draw_stat(actor_t * actor);

//Function of drawing a actor;
void draw_actors(actors_vt * actors, box_t box);

// Function for drawing features which lay on a terrain
// collect()'s items in a box and draws them in the correct places
void draw_features(features_vt * features, box_t box);

// Function which introduces messages and their history
void draw_text(int num, char * line);

//Function for initialisation
void init_GUI();

//Main function
//Every function above is called inside render()
void render(actor_t     * hero,
            actors_vt   * actors,
	 	        msgs_t      * msgs);

void close_windows();
