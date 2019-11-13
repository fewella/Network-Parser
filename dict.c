#include <stdlib.h>
#include <string.h>
#include <search.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "dict.h"

typedef struct char_to_int_node {
	char* key;
	int value;
} node_t;

int compar(const void* l, const void* r) {
	node_t* left = (node_t*)l;
	node_t* right = (node_t*)r;
	return strcmp((char*)left->key, (char*)right->key);
}

void del(char* k) {
	node_t* find = malloc(sizeof(node_t));
	find->key = strdup(k);
	void* p = tdelete(find, &root, compar);
	free(find);
}


void insert(char* k, int v) {
	if (get(k) != -1) {
		del(k);
	} 
	
	node_t* new_node = malloc(sizeof(node_t));
	new_node->key = strdup(k);
	new_node->value = v;
	tsearch(new_node, &root, compar);
}


int get(char* k) {
	node_t* find = malloc(sizeof(node_t));
	find->key = strdup(k);

	void* found = tfind(find, &root, compar);
	if (!found) {
		return 0;
	}
	
	int val = (*(node_t**)found)->value;
	free(find);
	return val;
}


