#pragma once
#include "GUI.h"
#include "common.h"
#include "kdtree.h"

level_t* init_level(int width,
                    int height);

bool update_level(level_t* level);

void free_level(level_t* level);
