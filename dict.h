void* root;

int compar(const void*, const void*);

/** Takes a key, removes the associated key-value pair
 */
void del(char*);


/** Inserts a key-value pair
 *  If it aleady exists, value will be replaced
 */
void insert(char* k, int v); 


/** Returns value associated with given key
 *  If it doesn't exist, will return 0
 */
int get(char* k);

