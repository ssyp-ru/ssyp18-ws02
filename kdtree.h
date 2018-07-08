#pragma once
#include "common.h"
#include <stdlib.h>
#include <math.h>

void buf (fvec_t * ret, feature_t * node);
fvec_t * collect(kdtree_t * root, box_t room);
void collect_rec (kdtree_t * tree, int num, box_t room, fvec_t * ret);
feature_t * findNN (kdtree_t* kdtree, feature_t * red, int num); 
kdtree_t * kd_create (feature_t ** point, int len);
kdtree_t * kd_create_node(feature_t * point);
kdtree_t * kd_insert (kdtree_t * tree, feature_t * point, int depth); 
void kd_delete (kdtree_t * root);
