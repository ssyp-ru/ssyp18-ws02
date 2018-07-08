#include "kdtree.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

feature_t nearest (feature_t * point, feature_t red) {
	feature_t curr = point[1];
	for (int i = 0; i<200; i++)
		if (hypot(curr.x-red.x, curr.y-red.y) > hypot(point[i].x-red.x, point[i].y-red.y))
			curr = point[i];
	return curr;
}

int main () {
	feature_t point[200];
	feature_t * ppoint[200];
	srand(time(NULL));
	for (int i = 0; i < 200; i++) {		
		point[i].x = rand() % 20;
		point[i].y = rand() % 20;
		ppoint[i] = &(point[i]);
	}

	kdtree_t * tree = kd_create (ppoint, 200);
	/*
	feature_t * nearkd = findNN (tree, &find, 1);
	feature_t nearf = nearest (point, find);
	if (nearf.x == nearkd->x && nearf.y == nearkd->y)
		printf ("yes");
	else
		printf ("no");

	for (int i = 1; i < 20; i++)
		printf ("%d, %d \n", point[i].x, point[i].y);
	*/
	box_t box;
	box.top = 5;
	box.bottom = 15;
	box.left = 5;
	box.right = 15;

	fvec_t * buf = collect(tree, box);
	printf("\n %d \n", buf->last);
	for (int i = 0; i < buf->last; i++)
		printf ("%d, %d \n", buf->buffer[i]->x, buf->buffer[i]->y);
	kd_delete (tree);

	return 0;
}
