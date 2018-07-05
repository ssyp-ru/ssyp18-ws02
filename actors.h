#pragma once

#include "common.h"


actors_t* initActors(map_t* map, int amountOfEntities);


bool behaveActors(map_t* map, actors_t* actors);


void drawActors(actors_t* actors);


void freeActors(actors_t* actors);
