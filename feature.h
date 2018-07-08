#pragma once
#include <ncurses.h>
#include "actor.h"
#include "common.h"

// Function for picking up items from a feature
void pick_up(actor_t * actor, feature_t feature);

//Function for dropping item on a floor
void throw_away(item_t * item, level_t * level, int x, int y);

//Function for interacting
void interact(feature_t * feature, actor_t * actor);
// feature_t * foo;
// foo = calloc(...);
// foo = interact_stairs;
// foo->interact(foo, some_actor);
