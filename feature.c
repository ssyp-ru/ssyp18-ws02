#include "feature.h"
#include "common.h"
#include "actor.h"
#include "kdtree.h"
#include <ncurses.h>

void pick_up(actor_t * actor, feature_t * feature){
	int length = feature->inventory->length;
	int item_count = actor->inventory->length;
	int max_count = actor->inventory->capacity;
	int index = -1;
	for(int i = 0; i < length && item_count <= max_count; i++){
		item_t item = feature->inventory->data[i];
		if(item.flags & FLAG_ITEM_STACKABLE){
			for(int k = 0; k < item_count; k++){
				if(actor->inventory->data[k].description == item.description){
					index = k;
				}
			}
			if(index != -1){
				actor->inventory->data[index].amount++;	
			} else {
				actor->inventory->length++;
				actor->inventory->data[item_count] = item;
				actor->inventory->data[item_count].amount = 1;
			}
		} else {
			actor->inventory->length++;
			actor->inventory->data[item_count] = item;
			actor->inventory->data[item_count].amount = 1;
		}
		item_count = actor->inventory->length;
	}
}

feature_t * find_feature (kdtree_t * tree, int x, int y, int depth) {
	int axis = depth%2;
	feature_t * found;
	if (tree) {
		if (axis == 1) {
			if (tree->node->x > x)
				found = find_feature (tree->rbranch, x, y, depth++);
			else
				found = find_feature (tree->lbranch, x, y, depth++);
		} else {	
			if (tree->node->y > y)
				found = find_feature (tree->rbranch, x, y, depth++);
			else
				found = find_feature (tree->lbranch, x, y, depth++);
		}
		return found;
	} else {
		return NULL;
	}
}

inventory_t * put_to_inv (inventory_t * inventory, item_t item) {
	inventory->length++;
	if (inventory->length <= inventory->capacity) {
		inventory->data[inventory->length] = item;	
	} else {
		inventory = realloc (inventory, sizeof(inventory_t *)*inventory->capacity*2);
		inventory->data[inventory->length] = item;	
	}
	return inventory;
}

void throw_away(item_t * item, actor_t * actor) {
	int place = 0;
	item_t * inv = actor->inventory->data;
	while (&inv[place] != item) // ?
		place++;
	inv[place] = inv[actor->inventory->length];
	actor->inventory->length--;
	
	feature_t * drop = find_feature(actor->level->features, actor->x, actor->y, 1);
	
	if (!drop) {
		drop->x = actor->x;
		drop->y = actor->y;
		drop->inventory = put_to_inv(drop->inventory, *item);
		actor->level->features = kd_insert (actor->level->features, drop, 1);
	} else 
		drop->inventory = put_to_inv(drop->inventory, *item);
}

void interact(actor_t * actor, feature_t * feature){
	switch(feature->type){
		case STAIRS:
			actor->level = feature->crutch->level;
			break;
		case CONTAINER:
		case DROPPED_ITEM:
			pick_up (actor, feature);
			break;
		case DEAD_BADGER:
		case NOTHING:
		case DOOR:
			break;
	}
}

