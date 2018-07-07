#pragma once

#include "common.h"

level_t* initLevel(int amountOfEntities, int width,
                   int height);

bool updateLevel(level_t* level);

void freeLevel(level_t* level);
