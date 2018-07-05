#pragma once


#include <stdbool.h>
#include "common.h"
#include "room.h"

void drawMap(map_t* map);


map_t* generateMap(int width, int height);


void freeMap(map_t* map);
