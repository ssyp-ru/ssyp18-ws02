typedef struct coor {
	int x;
	int y;
} coor_t;

typedef struct tree {
	tree_t * rbranch;
	tree_t * lbranch;
	coor_t node;
} tree_t;

coor_t findNN (tree_t tree, coor_t red, coor_t curr, coor_t cbest); 
