#include "kdtree.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

feature_t nearest (feature_t * point, feature_t red){
	feature_t curr = point[1];
	for (int i = 0; i<20; i++)
		if (hypot(curr.x-red.x, curr.y-red.y) > hypot(point[i].x-red.x, point[i].y-red.y))
			curr = point[i];
	return curr;
}

int main () {
	feature_t point[20];
	feature_t find;
	find.x = 0;
 	find.y = 0;
	for (int i = 0; i < 20; i++) {		
		point[i].x = rand();
		point[i].y = rand();
	}
	feature_t *ppoint[20];
	for(int i = 0; i < 20; i++) {
		ppoint[i] = &(point[i]);
	}
	kdtree_t * tree = kd_create (ppoint, 20);
	feature_t * nearkd = findNN (tree, &find, 1);
	feature_t nearf = nearest (point, find);
	if (nearf.x == nearkd->x && nearf.y == nearkd->y)
		printf ("yes");
	else
		printf ("no");
	kd_delete (tree);
	return 0;
}
