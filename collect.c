#include "kdtree.h"

void append(features_vt * ret, feature_t * node) {
	if (ret->size < ret->capacity) {
		ret->data[ret->size++] = node;
	} else {
		ret->data = realloc(ret->data, sizeof(feature_t *)*ret->capacity*2);
		ret->data[ret->size++] = node;
	}
}


void features_vector_destroy(features_vt * vec)
{
  free(vec->data);
  free(vec);
}

void collect_rec (kdtree_t * tree, int num, box_t room, features_vt * ret) {
	
	kdtree_t * right = tree->rbranch;
	kdtree_t * left = tree->lbranch;
	feature_t * curr = tree->node;
 	int axis = num%2;	
	
	if (axis == 1) {
		if (curr) {
			if (curr->x > room.x) {
				if (curr->x < room.x + room.width) {
          append(ret, curr);
					if (right)
						if (right->node->x < room.x + room.width)
							collect_rec (right, num+1, room, ret);
					if (left)
						if (left->node->x > room.x)
							collect_rec (left, num+1, room, ret);
		} else
			if (left)
				if (left->node->x > room.x)
					collect_rec (right, num+1, room, ret);
			}
		}
	} else {
		if (curr) {
			if (curr->y > room.y) {
				if (curr->y < room.y + room.height) {
          append(ret, curr);
					if (right)
						if (right->node->y < room.y + room.height)
							collect_rec (right, num+1, room, ret);
					if (left)
						if (left->node->y > room.y)
							collect_rec (left, num+1, room, ret);
				} else
					if (left)
						if (left->node->y > room.y)
							collect_rec (left, num+1, room, ret);
			}
		}
	}
}	

features_vt * collect(kdtree_t * root, box_t room) {
	features_vt * ret = calloc(1, sizeof(features_vt));
	ret->size = 0;
	ret->capacity = 16;
	ret->data = calloc(16, sizeof(feature_t *));
  if(root)
	  collect_rec (root, 1, room, ret);
	return ret;
}
