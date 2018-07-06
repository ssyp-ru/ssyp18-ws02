#pragma once
typedef struct tree_t {
	struct tree_t *left, *right, *parent;
	void* value;
} tree_t;
tree_t* tree_create (void* value);
void tree_add (tree_t* tree, void* value);
void tree_destroy(tree_t* tree);
tree_t* add_node(tree_t* tree, void* value);
void tree_dfs(tree_t* tree, void(*callback)(void*));
