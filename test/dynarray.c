#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "dynarray.h"

dynarray* dynarray_create(int nb_el_to_store, void* obj_ptr, int size_of_obj, bool (*fun)(void*, void*))
{
  if (nb_el_to_store < 1 || size_of_obj < 1 || obj_ptr == NULL)
    {
      return NULL;
    }

  dynarray *dynarr  = (dynarray*) malloc(1*sizeof(struct dyn));
  if (!dynarr)
    return NULL;

  dynarr->arr_el = (void**) malloc(nb_el_to_store*sizeof(obj_ptr));
  if (dynarr->arr_el == NULL)
    return NULL;
  
  dynarr->arr_size = nb_el_to_store;
  dynarr->size_of_obj = size_of_obj;
  dynarr->size_of_ptr = sizeof(obj_ptr);
  dynarr->el_count = 0;
  dynarr->cmp_fun = fun;
  
  return dynarr;
}

bool dynarray_add(dynarray *dynarr,  void *obj_to_add)
{
  if (obj_to_add == NULL || dynarr == NULL) {
    return false;
  }
    
  if ((dynarr->el_count) >= (dynarr->arr_size))
    {
      void **arr_tmp = (void**)malloc(2*(dynarr->arr_size)*(dynarr->size_of_ptr));
      if (!arr_tmp)
	return false;

      memcpy(arr_tmp, dynarr->arr_el, dynarr->arr_size);
      free(dynarr->arr_el);
      dynarr->arr_el = arr_tmp;
      dynarr->arr_size *= 2;
    }

  (dynarr->arr_el)[dynarr->el_count]  = (void*) malloc(1*(dynarr->size_of_obj));
  if (  (dynarr->arr_el)[dynarr->el_count]  == NULL)
    return false;
  
  memcpy((dynarr->arr_el)[dynarr->el_count], obj_to_add, dynarr->size_of_obj);

  dynarr->el_count++;
  
  return true;
}

bool dynarray_remove(dynarray *dynarr, int index_of_el_to_remove)
{
  int start_ind;

  if (dynarr == NULL ||
      index_of_el_to_remove < 0 ||
      index_of_el_to_remove >= dynarr->el_count)
    {
      return false;
    }

  for (start_ind = index_of_el_to_remove; start_ind < (dynarr->el_count - 1); start_ind++)
    {
      (dynarr->arr_el)[start_ind] = (dynarr->arr_el)[start_ind + 1];
    }

  dynarr->el_count--;
  
  return true;
}

int dynarray_search(dynarray *dynarr, void *obj_to_search)
{
  int start_ind;

  if (dynarr == NULL || obj_to_search == NULL) {
    return -1;
  }
  
  for (start_ind = 0; start_ind < dynarr->el_count; start_ind++)
    {
      if (dynarr->cmp_fun(obj_to_search, dynarr->arr_el[start_ind]) == true)
	{
	  return start_ind;
	}
    }

  return -1;
}

void* dynarray_element(dynarray *dynarr, int ind_of_obj)
{
  int start_ind;

  if (dynarr == NULL ||
      ind_of_obj < 0 ||
      ind_of_obj >= dynarr->el_count)
    {
      return NULL;
    }

  return dynarr->arr_el[ind_of_obj];
}

void* dynarray_find(dynarray *dynarr, void *obj_to_find)
{
  int start_ind;

  if (dynarr == NULL || obj_to_find == NULL)
    {
      return NULL;
    }

  for (start_ind = 0; start_ind < dynarr->el_count; start_ind++)
    {
      if (dynarr->cmp_fun(obj_to_find, dynarr->arr_el[start_ind]) == true)
	{
	  return dynarr->arr_el[start_ind];
	}
    }

  return NULL;
}

bool dynarray_isempty(dynarray *dynarr)
{
  if (dynarr == NULL)
    {
      return false;
    }

  if (dynarr->el_count == 0)
    {
      return true;
    }
  return false;
}

bool dynarray_isfull(dynarray *dynarr)
{
  if (dynarr == NULL)
    {
      return false;
    }
  
  if ((dynarr->el_count) == (dynarr->arr_size))
    {
      return true;
    }
  
  return false;
}

int dynarray_count(dynarray *dynarr)
{
  if (dynarr == NULL)
    {
      return -1;
    }

  return dynarr->el_count;
}

void dynarray_free(dynarray *dynarr)
{
  if (dynarr == NULL)
    {
      return;
    }
  
  free(dynarr->arr_el);
  free(dynarr);
}




