#pragma once
#include <ncurses.h>
#include "actor.h"
#include "common.h"

// Function for picking up items from a feature
void pick_up(actor_t * actor, feature_t feature);

//Function for dropping item on a floor
void throw_away(item_t * item, level_t * level, int x, int y);


