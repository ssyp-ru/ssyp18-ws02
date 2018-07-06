#pragma once

typedef struct coor {
	int x, y;
} coor_t;

typedef struct kdtree {
	struct kdtree * rbranch;
	struct kdtree * lbranch;
	coor_t node;
} kdtree_t;

coor_t findNN (kdtree_t* kdtree, coor_t red, int num); 
kdtree_t * kdcreate (coor_t * point, int depth, int i);
