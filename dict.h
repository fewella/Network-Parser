
// for del, insert, and get, need to pass in one of the following pointers as root (the first argument)
//void* outgoing;
void* root;

/**
 * Internal compare method for two keys
 */
int compar(const void*, const void*);

/** Takes a key, removes the associated key-value pair
 */
void del(int);


/** Inserts a key-value pair
 *  If it aleady exists, value will be replaced
 */
void insert(int k, int v); 


/** Returns value associated with given key
 *  If it doesn't exist, will return 0
 */
int get(int k);

/**
 * Goes through each node in the dict, makes a struct for each node (key->freq), and puts into array
 * Should also communicate with rshiny
 * IS THREAD SAFE
 */
void walk();

/**
 * Clears frequency of datapoints -  should be called after RShiny plots
 * IS THREAD SAFE
 */
void clear();
