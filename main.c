#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>


FILE* lf = NULL;

//chtype field[200][74]; // FIXME Global variable

typedef struct map_t {
	chtype* buffer;
	int height, wight;
} map_t;

#define UNPACK(varname, map_ptr) char (*varname)[(map_ptr)->wight] = (char (*)[(map_ptr)->wight]) map_ptr->buffer
typedef struct vector {
		void** data;
		size_t len;
		size_t maxLen;
} vector_t;

typedef struct room {
	int x1;
	int y1;
	int x2;
	int y2;
	int S;
	int lenX;
	int lenY;
} room_t;

typedef struct tree_t {
	struct tree_t *left, *right, *parent;
	void* value;
} tree_t;

void backTreeWalk (vector_t* vector) {
	
}

tree_t* tree_create (void* value) {
	tree_t* tree = malloc(sizeof(tree_t));
	tree->value = value;
	tree->left = NULL;
	tree->right = NULL;
	return tree;
}

vector_t* vector_init (int len) {
	vector_t* vector = malloc(sizeof(vector_t));
	vector->len = 0;
	if(len <= 0) len = 1;
	vector->maxLen = len;
	vector->data = calloc(len, sizeof(void*));
	return vector;
}

void vector_add (vector_t* v, void* value) {
	if(v->len >= v->maxLen) {
		v->maxLen = v->len * 2;
		void** data_help = (void**) realloc(v->data, sizeof(void*) * v->maxLen);
		if (data_help != NULL) {
			v->data = data_help;
		} else {
			// error
			return;
		}
	}
	v->data[v->len++] = value;
}

void* vector_get (vector_t* v, size_t i) {
	return v->data[i];
}

void vector_free(vector_t* v) {
	free(v->data);
	free(v);
}

vector_t* vector_copy (vector_t* v) {
	vector_t* vector = vector_init(v->maxLen);
	vector->len = vector->maxLen;
	memcpy(vector->data, v->data, v->len*sizeof(void*));
	return vector;
}

void tree_add (tree_t* tree, void* value) {
	if(tree == NULL) {
		fprintf(lf, "ERR: called tree_add() on NULL!\n");
		return;
	}
	if(tree->right == NULL) {
		tree->right = tree_create(value);
		tree->right->parent = tree;
	} else if(tree->left == NULL) {
		tree->left = tree_create(value);
		tree->left->parent = tree;
	} else {
		fprintf(lf, "ERR: Wrote third node to tree!\n");
	}
}
void tree_destroy(tree_t* tree) {
	if (tree != NULL) {
		tree_destroy(tree->left);
		tree_destroy(tree->right);
		free(tree);
	}
}

tree_t* add_node(tree_t* tree, void* value) {
	if (tree == NULL) {
		tree = tree_create(value);
	} else if(value < tree->value) {
		tree->right = add_node(tree->right, value);
	} else {
		tree->left = add_node(tree->left, value);
	}
	return tree;
}

void tree_dfs(tree_t* tree, void(*callback)(void*)) {
	if(tree != NULL) {
		tree_dfs(tree->right, callback);
		tree_dfs(tree->left, callback);
		callback(tree->value);
	}
}

void render (map_t* map, int scrollx, int scrolly) {
	UNPACK(field, map);
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 37; j++) {
			mvaddch(j, i, field[i +scrollx][j+scrolly]);
		}
	}
}

void room_free(room_t* room) {
	if(room != NULL) free(room);
}

void createLine (int x1, int y1, int x2, int y2, char ch, int COLOR, map_t* map) {
	UNPACK(field, map);
	double x=x1, y=y1;
	if (x1-x2 == 0) {
		double l = 0;
		double c = x1;
		while (y != y2) {
			x = l*y + c;
			field[(int)x][(int)y] = ch | COLOR_PAIR(COLOR);
			if (y2 >= y) {
				x++;
				y++;
			} else {
				x--;
				y--;
			}
		}
	} else {
		double k = (y1-y2)/(x1-x2);
		double b = y1 - k * x1;
		while (true) {
			if (x2 - x > y2 - y || y1 - y2 == 0) {
				y = k*x + b;
				x = round(x);
				y = round(y);
				field[(int)x][(int)y] = ch | COLOR_PAIR(COLOR);
				if (x2 >= x) {
					if (x >= x2) break;
					x++;
					y++;
				} else {
					if (x <= x2) break;
					x--;
					y--;
				}
			} else {
				x = (y - b)/k;
				x = round(x);
				y = round(y);
				field[(int)x][(int)y] = ch | COLOR_PAIR(COLOR);
				if (y2 >= y) {
					if (y >= y2) break;
					x++;
					y++;
				} else {
					if (y <= y2) break;
					x--;
					y--;
				}
			}
		}
	}
}
void divRoom (tree_t* parent, vector_t* leaves, map_t* map) {
	room_t* proom = parent->value;
	if(proom->S <= 64 || (proom->lenX <=16 && proom->lenY <= 8)) {
		vector_add(leaves, parent);
		return;
	}
	room_t* nrooms[2];
	nrooms[0] = malloc(sizeof(room_t));
	nrooms[1] = malloc(sizeof(room_t));
	int lenLast;
	int x1=0, y1=0, x2=0, y2=0;
	fflush (lf);
	if (proom->lenX > proom->lenY*2) {
		lenLast = proom->lenX;
		x1 = proom->x1 + (rand() %((lenLast*40)/100) + ((lenLast*30)/100));
		y1 = proom->y1;
		x2 = x1;
		y2 = proom->y2;
		fprintf(lf, "line x1=%d y1=%d x2=%d y2=%d\n", x1,y1,x2,y2);
		fprintf(lf, "x=%d y=%d\n", proom->lenX , proom->lenY);
		createLine(x1, y1, x2, y2, '#', 1, map);
		nrooms[1]->x2 = proom->x2;
		nrooms[1]->y2 = proom->y2;
		nrooms[1]->x1 = x2;
		nrooms[1]->y1 = y1;
		nrooms[0]->x2 = x2;
		nrooms[0]->y2 = y2;
		nrooms[0]->x1 = proom->x1;
		nrooms[0]->y1 = proom->y1;
	} else {
		lenLast = proom->lenY;
		y2 = proom->y2 + (rand() %((lenLast*40)/100) + ((lenLast*30)/100));
		x2 = proom->x2;
		y1 = y2;
		x1 = x2 - proom->lenX;
		fprintf(lf, "line x1=%d y1=%d x2=%d y2=%d\n", x1,y1,x2,y2);
		fprintf(lf, "x=%d y=%d\n", proom->lenX , proom->lenY);
		createLine(x1, y1, x2, y2, '#', 1, map);
		nrooms[1]->x2 = proom->x2;
		nrooms[1]->y2 = proom->y2;
		nrooms[1]->x1 = proom->x1;
		nrooms[1]->y1 = y2;
		nrooms[0]->x2 = x2;
		nrooms[0]->y2 = y2;
		nrooms[0]->x1 = proom->x1;
		nrooms[0]->y1 = proom->y1;
	}
	nrooms[0]->lenX = nrooms[0]->x2 - nrooms[0]->x1;
	nrooms[0]->lenY = nrooms[0]->y1 - nrooms[0]->y2;
	nrooms[0]->S = nrooms[0]->lenX * nrooms[0]->lenY;
	nrooms[1]->lenX = nrooms[1]->x2 - nrooms[1]->x1;
	nrooms[1]->lenY = nrooms[1]->y1 - nrooms[1]->y2;
	nrooms[1]->S = nrooms[1]->lenX * nrooms[1]->lenY;

	for (int i=0; i< 2; i++) {
		tree_add(parent, nrooms[i]);
	}
	divRoom(parent->right, leaves, map);
	divRoom(parent->left, leaves, map);
}
int main(int argc, char *argv[]) {
	map_t* map = (map_t*) malloc ( sizeof(map_t));
	lf = fopen("log.txt","w");
	fprintf(lf,"log opened\n");
	room_t* roomF = (room_t*) malloc ( sizeof(room_t));
	srand (time(NULL));
	initscr();
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_WHITE);
	init_pair(3, COLOR_YELLOW, COLOR_WHITE);
	UNPACK(field, map);
	for (int i = 0; i < 200; i++) {
		for (int j = 0; j < 74; j++) {
			field[i][j] = '.' | COLOR_PAIR(2);
		}
	}
	roomF->x1 = rand() % 15 + 30;
	roomF->y1 = rand() % 5 + 30;
	roomF->x2 = rand() % 15 + 70;
	roomF->y2 = rand() % 5 + 10;
	createLine(roomF->x1, roomF->y1, roomF->x2, roomF->y1, '#', 1, map);
	createLine(roomF->x1, roomF->y1, roomF->x1, roomF->y2, '#', 1, map);
	createLine(roomF->x1, roomF->y2, roomF->x2, roomF->y2, '#', 1, map);
	createLine(roomF->x2, roomF->y1, roomF->x2, roomF->y2, '#', 1, map);
	roomF->lenX = roomF->x2 - roomF->x1;
	roomF->lenY = roomF->y1 - roomF->y2;
	roomF->S = roomF->lenX * roomF->lenY;
	tree_t* root = tree_create(roomF);
	vector_t* leaves = vector_init(16);
	divRoom(root, leaves, map);
	render(map, 0, 0);
	refresh();
	fclose(lf);
	tree_dfs(root, room_free);
	vector_free(leaves);
	tree_destroy(root);
	getch();
	endwin();
	return 0;
}

