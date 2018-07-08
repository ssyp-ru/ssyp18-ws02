#include "kdtree.h"

features_vt * collect(kdtree_t * root, box_t room) {
	// TODO: We create an instance of features_vt and pass it to collect_rec()
	// by pointer. 
	features_vt * ret = calloc(1, sizeof(features_vt));
	if (ret->last < ret->capacity) {
		ret->buffer[ret->last++] = root->node; //
	} else {
		ret->buffer = realloc(ret->buffer, sizeof(ret->buffer)*2);
	}
	collect_rec (root, 1, room, ret);
	return ret;
}

void collect_rec (kdtree_t * tree, int num, box_t room, features_vt * ret) {
	
	kdtree_t * right = tree->rbranch;
	kdtree_t * left = tree->lbranch;
	feature_t curr = tree->node;
 	int axis = num%2;	
	
	if (axis == 1) {
		if (curr.x > room.left) {
			if (curr.x < room.right) {
				ret->buffer[ret->last] = curr; //
				if (right->node.x < room.right)
					collect_rec (right, num+1, room, ret);
				if (left->node.x > room.left)
					collect_rec (left, num+1, room, ret);
		} else
			if (left->node.x > room.left)
				collect_rec (right, num+1, room, ret);
		}
	} else {
		if (curr.y > room.top) {
			if (curr.y < room.bottom) {
				ret->buffer[ret->last] = curr; //
				if (right->node.y < room.bottom)
					collect_rec (right, num+1, room, ret);
				if (left->node.y > room.top)
					collect_rec (left, num+1, room, ret);
			} else
				if (left->node.y > room.top)
					collect_rec (left, num+1, room, ret);
		}
	}
}	
