#pragma once
#include <stdlib.h>
#include <math.h>
#include "common.h"

features_vt * collect(kdtree_t * root, box_t room);
void features_vector_destroy(features_vt * vec);

feature_t * findNN (kdtree_t* kdtree, feature_t * red, int num); 
kdtree_t * kd_create (feature_t ** point, int len);
kdtree_t * kd_check (kdtree_t * root, int x, int y);
kdtree_t * kd_create_node(feature_t * point);
kdtree_t * kd_insert (kdtree_t * tree, feature_t * point, int depth); 
void kd_delete (kdtree_t * root);
kdtree_t * kd_remove (kdtree_t * root, feature_t * node, int depth);
