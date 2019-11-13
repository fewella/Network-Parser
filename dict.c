#include <search.h>

#include "dict.h"

typedef struct char_to_int_node {
	char* key;
	int value;
} node_t;

int compar(const void* l, const void* r) {
	return strcmp((char*)l, (char*)r);
}

void del(char* k) {
	node_t* find = malloc(sizeof(node_t));
	find->key = k;
	void* p = tdelete(find, &root, compar);
	free(find);
	printf("del ret: %p\n", p);
}


void insert(char* k, int v) {
	if (get(k) != -1) {
		del(k);
	} 
	
	node_t* new_node = malloc(sizeof(node_t));
	new_node->key = k;
	new_node->value = v;
	tsearch(new_node, &root, compar);
}


int get(char* k) {
	node_t* find = malloc(sizeof(node_t));
	find->key = k;

	void* found = tfind(find, &root, compar);
	if (!found) {
		return 0;
	}
	
	int val = (*(node_t**)found)->value;
	free(find);
	return val;
}

