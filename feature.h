#pragma once
#include <ncurses.h>
#include "actor.h"
#include "common.h"

// Function for picking up items from a feature
void pick_up(actor_t * actor, feature_t * feature);

//If there is a feature 
feature_t * find_feature (kdtree_t * tree, int x, int y, int depth);

//Adding item to inventory. For features
inventory_t * put_to_inv (inventory_t * inventory, item_t item);

//Function for dropping item on a floor
void throw_away(item_t * item, actor_t * actor);

//Function for interacting
void interact(actor_t * actor, feature_t * feature);
// feature_t * foo;
// foo = calloc(...);
// foo = interact_stairs;
// foo->interact(foo, some_actor);
