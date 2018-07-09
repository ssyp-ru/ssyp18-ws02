#include "common.h"
#include "levelvector.h"
#include "kdtree.h"
#include "time.h"
#include "genmap.h"

/*typedef struct feature{
	struct inventory * inventory;
	ftype_t type;
	int x, y;
	int flags;
	char * description;
	chtype symbol;
	void (*interact)(struct feature *, struct actor *);
} feature_t;
*/
features_vt * prototypes = NULL;

void free_protofeatures() {
  for(int i = 0; i < prototypes->size; i++)
    free(prototypes->data[i]);
	free(prototypes->data);
	free(prototypes);
}

void init_protofeatures() {

	init_pair(5, COLOR_GREEN,   COLOR_BLACK);
	init_pair(6, COLOR_WHITE,   COLOR_CYAN);
	init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
	
	features_vt * lib = calloc(1, sizeof(features_vt));
	
	lib->capacity = 10;
	lib->data	= calloc(lib->capacity, sizeof(feature_t *));

	lib->data[0] = calloc(1, sizeof(feature_t));
	lib->data[0]->type = NOTHING;
	lib->data[0]->description = "Stone statue of Cthulhu";
	lib->data[0]->symbol='&' |COLOR_BROWN;

	lib->data[1] = calloc(1, sizeof(feature_t));
	lib->data[1]->type = NOTHING;
	lib->data[1]->description = "Goblin corpse";
	lib->data[1]->symbol='%' | COLOR_BROWN;
	lib->data[1]->flags = FLAG_FEATURE_EDIBILITY; 

	lib->data[2] = calloc(1, sizeof(feature_t));
	lib->data[2]->type = NOTHING;
	lib->data[2]->description = "Mammoth skeleton";
	lib->data[2]->symbol='/' | COLOR_BROWN;

	lib->data[3] = calloc(1, sizeof(feature_t));
	lib->data[3]->type = NOTHING;
	lib->data[3]->description = "Battle axe";
	lib->data[3]->symbol='L' | COLOR_BROWN;
	lib->data[3]->flags = FLAG_FEATURE_WEAPON;

	lib->data[4] = calloc(1, sizeof(feature_t));
	lib->data[4]->type = NOTHING;
	lib->data[4]->description = "Leaky chain mail pants";
	lib->data[4]->symbol='v' | COLOR_BROWN;
	lib->data[4]->flags = FLAG_FEATURE_CLOTHES;

	lib->data[5] = calloc(1, sizeof(feature_t));
	lib->data[5]->type = NOTHING;
	lib->data[5]->description = "The throne of skulls";
	lib->data[5]->symbol='h' | COLOR_BROWN;
//изолента, красные демоны, подбирающие черепа, трон из черепов, лужа крови

	lib->data[6] = calloc(1, sizeof(feature_t));
	lib->data[6]->type = CONTAINER;
	lib->data[6]->description = "Oak chest";
	lib->data[6]->symbol=']' | COLOR_BROWN;
	
	lib->data[7] = calloc(1, sizeof(feature_t));
	lib->data[7]->type = CONTAINER;
	lib->data[7]->description = "Cardboard box";
	lib->data[7]->symbol=']' | COLOR_BROWN;

	lib->size = 8;
	prototypes = lib;
}
	
void gen_feature(level_t * level, int num) {
	srand(time(NULL));
	UNPACK(_map, level->map);
	int x, y, feat_num;
	for(int i = 0; i < num; i++) {
		do {
			feat_num = rand() % prototypes->size;
			x = rand() % level->map->width;
			y = rand() % level->map->height; 
		} while(_map[x][y].flags & FLAG_TILE_SOLID);
	 	feature_t * feat = calloc(1, sizeof(feature_t));
		*feat = *(prototypes->data[feat_num]);
		feat->x = x;
		feat->y = y;
		feat->level = level;
		level->features = kd_insert(level->features, feat, 1);
	}
}

void gen_stair(levels_vt * levels, int num) {
	int lev1, lev2, x1, x2, y1, y2;
	for(int i = 0; i < num; i++) {
		lev1 = rand() % levels->length;

		do {
			lev2 = rand() % levels->length;
		} while(lev1 == lev2);
		
		UNPACK(map1, levels->data[lev1]->map);
		UNPACK(map2, levels->data[lev2]->map); 
		do {
			x1 = rand() % levels->data[lev1]->map->width;
			y1 = rand() % levels->data[lev1]->map->height; 
		}while(map1[x1][y1].flags & FLAG_TILE_SOLID);
		
		do {
			x2 = rand() % levels->data[lev2]->map->width;
			y2 = rand() % levels->data[lev2]->map->height; 
		}while(map2[x2][y2].flags & FLAG_TILE_SOLID);

		feature_t * stair1 = calloc(1, sizeof(feature_t));
		stair1->type = STAIRS;
		stair1->description = "Stair";
		stair1->x = x1;
		stair1->y = y1;
		stair1->level = levels->data[lev1];

		feature_t * stair2 = calloc(1, sizeof(feature_t));
		stair2->type = STAIRS;
		stair2->description = "Stair";
		stair2->x = x2;
		stair2->y = y2;
		stair2->level = levels->data[lev2];
		
		stair1->crutch = stair2;
		stair2->crutch = stair1;

		int orientation = rand()%2;
		if(orientation == 0) { 
			stair1->symbol = '<';
			stair2->symbol = '>';
		}
		else {
			stair1->symbol = '>';
			stair2->symbol = '<';
		}
		levels->data[lev1]->features = kd_insert(levels->data[lev1]->features, stair1, 1);
		levels->data[lev2]->features = kd_insert(levels->data[lev2]->features, stair2, 1);
	}	 		
}

