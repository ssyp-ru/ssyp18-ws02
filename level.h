#pragma once
#include "GUI.h"
#include "common.h"

level_t* init_level(int width,
                    int height);

bool update_level(level_t* level);

void draw_level(level_t* level,box_t box, actor_t * actor,
	 	feature_t * features,	msgs_t * msgs); 

void free_level(level_t* level);
