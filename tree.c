#include "tree.h"

coor_t findNN (tree_t tree, coor_t red, coor_t curr, coor_t cbest) {
	if (tree.rbranch != null && tree.lbranch != null) {
		if (red.x > tree.node.x)
			findNN (tree.lbranch, red, tree.lbranch.node, cbest);
		else
			findNN (tree.rbranch, red, tree.rbranch.node, cbest);
	}

	int M = dist(red.x, red.y, curr.x, curr.y);
	int min = dist(red.x, red.y, cbest.x, cbest.y);
	
	if (M >= min) {
		findNN (tree.rbranch, red, tree.rbranch.node, cbest);
	}
	if (M < min) {
		cbest = curr;
		findNN (tree.lbranch, red, tree.lbranch.node, cbest);
	}
	return cbest;
}
