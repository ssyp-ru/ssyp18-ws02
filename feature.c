#include "feature.h"
#include "common.h"
#include "actor.h"
#include <ncurses.h>

void pick_up(actor_t * actor, feature_t * feature){
	int length = feature->inventory->length;
	int item_count = actor->inventory->length;
	int max_count = actor->inventory->capacity;
	int index = -1;
	for(int i = 0; i < length && item_count <= max_count; i++){
		item_t item = feature->inventory->data[i];
		if(item.flags & FLAG_STACKABLE){
			for(int k = 0; k < item_count; k++){
				if(actor->inventory->data[k].description == item.description){
					index = k;
				}
			}
			if(index != -1){
				actor->inventory->data[index].amount++;	
			}
			else{
				actor->inventory->length++;
				actor->inventory->data[item_count] = item;
				actor->inventory->data[item_count].amount= 1;
			}
		}
		else{
			actor->inventory->length++;
			actor->inventory->data[item_count] = item;
			actor->inventory->data[item_count].amount= 1;
		}
		item_count = actor->inventory->length;
	}
}

void throw_away(item_t * item, level_t * level, int x, int y){

}
/*void interact(actor_t actor, feature_t feature){
	switch(feature.ftype_t){
		case UPSTAIRS:
			actor->
			break;

	}
}
*/
