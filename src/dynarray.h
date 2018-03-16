#ifndef DYNARRAY_H
#define DYNARRAY_H

#include <stdlib.h>

typedef struct dyn
{
  void **arr_el;
  bool (*cmp_fun)(void*, void*);
  int size_of_obj;
  int size_of_ptr;  
  long long el_count;
  long long arr_size;
}dynarray;

dynarray* dynarray_create(int nb_el_to_store, void* obj_ptr, int size_of_obj, bool (*fun)(void*, void*));
bool dynarray_add(dynarray *dynarr,  void *obj_to_add);
bool dynarray_remove(dynarray *dynarr, int index_of_el_to_remove);
void* dynarray_element(dynarray *dynarr, int ind_of_obj);
int dynarray_search(dynarray *dynarr, void *obj_to_search);
void* dynarray_find(dynarray *dynarr, void *obj_to_find);
bool dynarray_isempty(dynarray *dynarr);
bool dynarray_isfull(dynarray *dynarr);
int dynarray_count(dynarray *dynarr);
void dynarray_free(dynarray *dynarr);

#endif
