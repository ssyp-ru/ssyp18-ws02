#pragma once

#include "common.h"

level_t* init_level(int width,
                   int height);

bool update_level(level_t* level);

void draw_level(level_t* level);

void free_level(level_t* level);
