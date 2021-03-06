#include <malloc.h>
#include <math.h>
#include "kdtree.h"

feature_t * findNN (kdtree_t * kdtree, feature_t * red, int num)
{
	feature_t * curr;
	kdtree_t * ok_branch, * other_branch;
	if (num%2) { // x axis
		if (red->x < kdtree->node->x) {
			ok_branch = kdtree->lbranch;
			other_branch = kdtree->rbranch;
		} else {
			ok_branch = kdtree->rbranch;
			other_branch = kdtree->lbranch;
		}

		if (ok_branch == NULL)
			return kdtree->node;
		else
			curr = findNN (ok_branch, red, num+1);

		double M = hypot(red->x - kdtree->node->x, red->y - kdtree->node->y);
		double split_dist = abs(red->x - kdtree->node->x);

		if (split_dist < M) {
			feature_t * other = findNN (other_branch, red, num+1); // !!!
			if (hypot(red->x - other->x, red->y - other->y) < hypot(red->x - curr->x, red->y - curr->y))
				curr = other;
		}
		return curr;

	}	else { // y axis
		if (red->y < kdtree->node->y) {
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
		double M = hypot(red->x - kdtree->node->x, red->y - kdtree->node->y);
		double split_dist = abs(red->y - kdtree->node->y);

		if (split_dist < M)
		{
			feature_t * other = findNN (other_branch, red, num+1); // !!!
			if (abs(red->y - other->y) < abs(red->y - curr->y))
				curr = other;
		}
	}
	return curr;
}


kdtree_t * kd_create_node(feature_t * point) {
	kdtree_t * ret = malloc(sizeof(kdtree_t));
	ret->lbranch = NULL;
	ret->rbranch = NULL;
	ret->node = point;
	return ret;
}

kdtree_t * kd_insert (kdtree_t * tree, feature_t * point, int depth) {
	if (!tree)
	{
		return kd_create_node(point);	
	}
	kdtree_t * parent = tree;
	kdtree_t * child = kd_create_node(point);
	do {
		if (depth++%2) {
			if (parent->node->x > point->x) {
				if (parent->lbranch) {
					parent = parent->lbranch;
          continue;
        } else {
					parent->lbranch = child;
					return tree;
				}
      } else {
				if (parent->rbranch)
					parent = parent->rbranch;
				else {
					parent->rbranch = child;
					return tree;
				}
      }
		} else {
			if (parent->node->y > point->y) {
				if (parent->lbranch)
					parent = parent->lbranch;
				else {
					parent->lbranch = child;
					return tree;
				}
			} else {
				if (parent->rbranch)
					parent = parent->rbranch;
				else {
					parent->rbranch = child;
					return tree;
				}
      }
		}
	} while(1);
}

kdtree_t * kd_create (feature_t ** point, int len) {
	kdtree_t * tree = NULL;
	int i = 0;
	while (i < len) {
		tree = kd_insert (tree, point[i], 0); 
		i++;
	}
	return tree;
}

void kd_delete (kdtree_t * root) {
	if (root) {
		if (root->rbranch)
			kd_delete (root->rbranch);
		if (root->lbranch) 
			kd_delete (root->lbranch);
    free(root->node);
  	free (root);
	}
}

kdtree_t * kd_check(kdtree_t * root, int x, int y) {
 	if (!root)
	{
		return NULL;
	}
  int depth = 1;
	kdtree_t * parent = root;
	do {
    if (parent->node->x == x
        && parent->node->y == y)
        return parent; //FIXME: Support more than 1 feature per tile.
		if (depth++%2) {
			if (parent->node->x > x) {
				if (parent->lbranch) {
					parent = parent->lbranch;
        } else break;
      } else {
				if (parent->rbranch)
					parent = parent->rbranch;
				else break;
      }
		} else {
			if (parent->node->y > y) {
				if (parent->lbranch)
					parent = parent->lbranch;
				else break;
			} else {
				if (parent->rbranch)
					parent = parent->rbranch;
				else break;
      }
		}
	} while(parent->rbranch || parent->lbranch); 
  return NULL;
}

kdtree_t * kd_remove (kdtree_t * root, feature_t * node, int depth) {
	int axis = depth%2;
	if (!root) {
		return NULL;
	}
  if (root->node->fid != node->fid) {
    if (axis == 1) {
			if (node->x > root->node->x) { //valgrind error
				if (root->rbranch)
					root = kd_remove (root->rbranch, node, depth++);
			} else {
				if (root->lbranch)
					root = kd_remove (root->lbranch, node, depth++);
			}
		}	else {	
			if (node->y > root->node->y) {
				if (root->rbranch)
					root = kd_remove (root->rbranch, node, depth++);
			}	else {
				if (root->lbranch)
					root = kd_remove (root->lbranch, node, depth++);
			}
		}
	}
	kdtree_t * rem = root;
	if (root->lbranch) {
	 if (root->rbranch)
		 kd_remove (root->rbranch, node, depth++);
	} else {
		if (root->lbranch)
			kd_remove (root->lbranch, node, depth++);
	}
	rem = root;
	free (root);
	return rem;
}

