#pragma once

#include "common.h"

level_t* init_level(int amount_of_entities, int width,
                   int height);

bool update_level(level_t* level);

void free_level(level_t* level);
