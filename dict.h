#include <search.h>

void* root;

typedef struct char_to_int_node {
	char* key;
	int value;
} node_t;

int compar(const void* l, const void* r) {
	return strcmp((char*)l, (char*)r);
}

void insert(char* k, int v) {
	node_t* new_node = malloc(sizeof(node_t));
	new_node->key = k;
	new_node->value = v;
	tsearch(new_node, &root, compar);
}

int get(char* k) {
	node_t* find = malloc(sizeof(node_t));
	find->key = k;

	void* found = tfind(find, &root, compar);
	int val = (*(node_t**)found)->value;
	free(find);
	return val;
}

