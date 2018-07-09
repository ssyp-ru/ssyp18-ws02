#include "common.h"
#include "levelvector.h"
#include "kdtree.h"
#include "time.h"

extern features_vt * prototypes;

void free_protofeatures();

void init_protofeatures();
	
void gen_feature(level_t * level, int num);

void gen_stair(levels_vt * levels, int num);
