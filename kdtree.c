#include <stdlib.h>
#include <math.h>
#include "kdtree.h"

coor_t findNN (kdtree_t *kdtree, coor_t red, int num) {
	coor_t curr;
	kdtree_t * ok_branch, * other_branch;
	if (num%2) { // x axis
		if (red.x < kdtree->node.x) {
			ok_branch = kdtree->lbranch;
			other_branch = kdtree->rbranch;
		} 
		else {
			ok_branch = kdtree->rbranch;
			other_branch = kdtree->lbranch;
		}
		
		if (ok_branch == NULL)
			return kdtree->node;
		else
			curr = findNN (ok_branch, red, num+1);
		double M = hypot(red.x - kdtree->node.x, red.y - kdtree->node.y);
		double split_dist = abs(red.x - kdtree->node.x);

		if (split_dist < M) {
			coor_t other = findNN (other_branch, red, num+1);
		// FIXME: real hypot
		if (hypot(red.x - other.x) < hypot(red.x - curr.x))
			curr = other;
		}
		return curr;
	}	else { // y axis
		if (red.y < kdtree->node.y) {
			ok_branch = kdtree->lbranch;
			other_branch = kdtree->rbranch;
		}	else {
			ok_branch = kdtree->rbranch;
			other_branch = kdtree->lbranch;
		}
		if (ok_branch == NULL)
			return kdtree->node;
		else
			curr = findNN (ok_branch, red, num+1);
		double M = hypot(red.x-kdtree->node.x, red.y - kdtree->node.y);
		double split_dist = abs(red.y - kdtree->node.y);

		if (split_dist < M) {
			coor_t other = findNN (other_branch, red, num+1);
		if (abs(red.y-other.y) < abs(red.y-curr.y))
			curr = other;
		}
		return curr;
	}	
}

kdtree_t * kdcreate (coor_t * point, int depth, int i) {
	int axis = depth%2;
	kdtree_t * tree;
		if (axis == 1){ 
			tree->node = point[i];
			if (point[i+1].y > point[i].y)
				tree->rbranch = kdcreate (point, depth+1, i+1);
			else
				tree->lbranch = kdcreate (point, depth+1, i+1);
		}
		else{ 
			tree->node = point[i];
			if (point[i+1].x > point[i].x)
				tree->rbranch = kdcreate (point, depth+1, i+1);
			else
				tree->lbranch = kdcreate (point, depth+1, i+1);
		}
		return tree;
}
