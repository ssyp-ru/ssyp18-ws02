#include "common.h"
#include "levelvector.h"
#include "kdtree.h"
#include "time.h"

#define FLAG_FEATURE_PERSISTENT 0x1
#define FLAG_FEATURE_EDIBILITY  0x2
#define FLAG_FEATURE_CLOTHES    0x4
#define FLAG_FEATURE_WEAPON     0x8

features_vt * prototypes = NULL;

void free_protofeatures();

void init_protofeatures();
	
void gen_feature(level_t * level, int num);

void gen_stair(levels_vt * levels, int num);
