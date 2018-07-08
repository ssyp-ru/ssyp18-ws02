#include "feature.h"
#include "common.h"
#include "actor.h"
#include <ncurses.h>

void pick_up(actor_t * actor, feature_t feature){
	int amount = feature.inventory->amount;
	int item_count = actor->inventory->amount;
	int max_count = actor->inventory->max_amount;
	int x = feature.x;
	int y = feature.y;
	int index = -1;
	for(int i = 0; i < amount && item_count <= max_count; i++){
		item_t item = feature.inventory->item[i];
		if(item.flags & FLAG_STACKABLE){
			for(int k = 0; k < item_count; k++){
				if(actor->inventory->item[k].description == item.description){
					index = k;
				}
			}
			if(index != -1){
				actor->inventory->item[index].amount++;	
			}
			else{
				actor->inventory->amount++;
				actor->inventory->item[item_count] = item;
				actor->inventory->item[item_count].amount = 1;
			}
		}
		else{
			actor->inventory->amount++;
			actor->inventory->item[item_count] = item;
			actor->inventory->item[item_count].amount = 1;
		}
		item_count = actor->inventory->amount;
	}
}

void throw_away(item_t * item, level_t * level, int x, int y){

}
/*void interract(actor_t actor, feature_t feature){
	switch(feature.ftype_t){
		case UPSTAIRS:
			actor->
			break;

	}
}
*/
