#include "rooms.h"
#include <stdlib.h>
tree_room_t* tree_create (void* value) {
	tree_room_t* tree = malloc(sizeof(tree_room_t));
	tree->value = value;
	tree->left = NULL;
	tree->right = NULL;
	return tree;
}
void tree_add (tree_room_t* tree, void* value) {
	if(tree == NULL) {
		return;
	}
	if(tree->right == NULL) {
		tree->right = tree_create(value);
		tree->right->parent = tree;
	} else if(tree->left == NULL) {
		tree->left = tree_create(value);
		tree->left->parent = tree;
	} else {
	}
}
void tree_destroy(tree_room_t* tree) {
	if (tree != NULL) {
		tree_destroy(tree->left);
		tree_destroy(tree->right);
		free(tree);
	}
}
tree_room_t* add_node(tree_room_t* tree, void* value) {
	if (tree == NULL) {
		tree = tree_create(value);
	} else if(value < tree->value) {
		tree->right = add_node(tree->right, value);
	} else {
		tree->left = add_node(tree->left, value);
	}
	return tree;
}
void tree_dfs(tree_room_t* tree, void(*callback)(void*)) {
	if(tree != NULL) {
		tree_dfs(tree->right, callback);
		tree_dfs(tree->left, callback);
		callback(tree->value);
	}
}
