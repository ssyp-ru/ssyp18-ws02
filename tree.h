#pragma once
typedef struct coor {
	int x;
	int y;
} coor_t;

typedef struct tree {
	tree_t * rbranch;
	tree_t * lbranch;
	coor_t node;
} tree_t;
