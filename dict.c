#include <stdlib.h>
#include <string.h>
#include <search.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

#include "dict.h"
#include "capture.h"

typedef struct int_to_int_node {
	int key;
	int value;
} node_t;

int compar(const void* l, const void* r) {
	node_t* left = (node_t*)l;
	node_t* right = (node_t*)r;
	return left->key - right->key;
}


void del(int k) {
	node_t* find = malloc(sizeof(node_t));
	find->key = k;
	void* p = tdelete(find, &root, compar);
	free(find);
}


void insert(int k, int v) {
	if (get(k) != -1) {
		del(k); // this is extremely expensive - try just modifying the returned node instead
	} 
	
	node_t* new_node = malloc(sizeof(node_t));
	new_node->key = k;
	new_node->value = v;
	tsearch(new_node, &root, compar);
}


int get(int k) {
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

void walk() {
	int i;
	pthread_mutex_lock(&m);
	printf("WALKING, datapoints addr: %p\n", datapoints);
	for (i = 0; i < NUM_KEYS; i++) {
		printf("i-1: %d\n", i-1);
		datapoints[i].key  = i - 1;
		datapoints[i].freq = get(i - 1);
		printf("key, freq: %d, %f\n", datapoints[i].key, datapoints[i].freq);
	}
	pthread_mutex_unlock(&m);
}

void clear() {
	int i;
	pthread_mutex_lock(&m);
	for (i = 0; i < NUM_KEYS; i++) {
		datapoints[i].freq = 0;
	}
	pthread_mutex_unlock(&m);
}


