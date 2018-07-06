#pragma once
#include "common.h"
#include "hero.h"

// Structure of coordinates
typedef struct coord{
	int xPos;
	int yPos;
}coord_t;

// Structure of vision
typedef struct vision{
	coord_t * view;
}vision_t;

// Map-drawing function
// Renders a chunk of _map bounded by a box;
void drawMap(map_t * _map);

// Function for drawing a field of view
void drawView(int xPos, int yPos, int viewRadius, map_t * _map);

//Function for drawing an inventory
void drawInv(hero_t * hero);

//Function for drawing statistics
void drawStats(hero_t * hero);

//Function of drawing a hero;
void drawHero(hero_t * hero);

// Function for drawing features which lay on a terrain
// collect()'s items in a box and draws them in the correct places
//void drawFeatures(kdtree_t * features, WINDOW * window);

// Function which introduces messages and their history
void drawText(char * line);

//Function for initialisation
void initGUI(map_t * _map);

//Main function
//Every function above is called inside render()
void render(map_t * _map, hero_t * hero);
