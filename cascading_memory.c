#include <stddef.h>
#include <stdlib.h>
#include <R.h>

typedef void* reducer_func(void* array, size_t element_size);

struct _cascade_meta {
  size_t capacity;
  size_t count;
  size_t resize_threshold;
  size_t element_size;
  reducer_func* func;
};

typedef struct _cascade_meta cascade_meta;

struct _cascade {
  cascade_meta* meta;
  void* array;
  struct _cascade* child;
};

typedef struct _cascade cascade;

cascade* cascade_create(
    cascade* child, 
    
  ) {
  
  cascade* this = malloc(sizeof(cascade));
  this->array = malloc(element_size * capacity);
  this->capacity = capacity;
  this->count = 0;
  this->child = child;
  return this;
}




