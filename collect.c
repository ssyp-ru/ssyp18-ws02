#include "kdtree.h"

void buf (fvec_t * ret, feature_t * node) {
	if (ret->last < ret->capacity) {
		ret->buffer[ret->last++] = node;
	} else {
		ret->buffer = realloc(ret->buffer, sizeof(feature_t *)*ret->capacity*2);
		ret->buffer[ret->last++] = node;
	}
}

fvec_t * collect(kdtree_t * root, box_t room) {
	fvec_t * ret = calloc(1, sizeof(fvec_t));
	ret->last = 0;
	ret->capacity = 16;
	ret->buffer = calloc(16, sizeof(feature_t *));
	buf (ret, root->node);
	collect_rec (root, 1, room, ret);
	return ret;
}

void collect_rec (kdtree_t * tree, int num, box_t room, fvec_t * ret) {
	
	kdtree_t * right = tree->rbranch;
	kdtree_t * left = tree->lbranch;
	feature_t * curr = tree->node;
 	int axis = num%2;	
	
	if (axis == 1) {
		if (curr) {
			if (curr->x > room.left) {
				if (curr->x < room.right) {
					ret->buffer[ret->last++] = curr;
					if (right)
						if (right->node->x < room.right)
							collect_rec (right, num+1, room, ret);
					if (left)
						if (left->node->x > room.left)
							collect_rec (left, num+1, room, ret);
		} else
			if (left)
				if (left->node->x > room.left)
					collect_rec (right, num+1, room, ret);
			}
		}
	} else {
		if (curr) {
			if (curr->y > room.top) {
				if (curr->y < room.bottom) {
					ret->buffer[ret->last++] = curr;
					if (right)
						if (right->node->y < room.bottom)
							collect_rec (right, num+1, room, ret);
					if (left)
						if (left->node->y > room.top)
							collect_rec (left, num+1, room, ret);
				} else
					if (left)
						if (left->node->y > room.top)
							collect_rec (left, num+1, room, ret);
			}
		}
	}
}	
