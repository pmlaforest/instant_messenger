#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "dynarray.h"

typedef struct
{
  int integer;
  float floating_float;
  double double_test;
}test_struct;
 
test_struct test_struct1 = {1, 1.00, 3.65645656464956565};
test_struct test_struct2 = {3, 2.00, 3.65645656464956565};

bool compare(void *el1, void *el2)
{
  test_struct *el1_struct;
  test_struct *el2_struct;

  el1_struct = (test_struct*)el1;
  el2_struct = (test_struct*)el2;

  if (el1_struct->integer == el2_struct->integer)
    {
      return true;
    }
  
  return false;
}

bool test1_dynarray_create(int nb_of_array, int size_of_array)
{
  dynarray *new_array;

  for (int i = 0; i < nb_of_array; i++)
    {
      new_array = dynarray_create(size_of_array, (void*)&test_struct1, sizeof(test_struct), compare);
      if (new_array == NULL) {
	return false;
      }
    }

  return true;
}

bool test1_dynarray_add(int nb_of_elements_in_arr, int nb_of_elements_to_add)
{
  dynarray *new_array;

  if ((new_array = dynarray_create(nb_of_elements_in_arr, (void*)&test_struct1, sizeof(test_struct1), compare)) == false)
    return false;

  for (int i = 0; i < nb_of_elements_to_add; i++)
    {
      if (dynarray_add(new_array,(void*)&test_struct1) == false)
	return false;
    }

  return true;
}

bool test1_dynarray_remove(int nb_of_elements_in_arr, int index_of_el_to_remove)
{
  dynarray *new_array;

  new_array = dynarray_create(nb_of_elements_in_arr, (void*)&test_struct1, sizeof(test_struct1), compare);

  if (dynarray_add(new_array,(void*)&test_struct1) == false)
    return false;

  if (dynarray_remove(new_array, index_of_el_to_remove) == false)
    return false;
  
  return true;
}

bool test1_dynarray_element(int nb_of_el_in_ar, int ind_of_obj)
{
  dynarray *new_array;

  new_array = dynarray_create(nb_of_el_in_ar, (void*)&test_struct1, sizeof(test_struct1), compare);

  if (dynarray_add(new_array,(void*)&test_struct1) == false)
    return false;
  
  if (dynarray_element(new_array, ind_of_obj) == false)
    return false;

  return true;
}

bool test1_dynarray_search(void *obj_to_search, int size_of_dynarray)
{
  dynarray *new_array;

  new_array = dynarray_create(1, (void*)&test_struct1, sizeof(test_struct1), compare);

  if (dynarray_add(new_array, (void*)&test_struct1) == false)
    return false;
  
  if (dynarray_search(new_array, obj_to_search) == -1)
    {
      return false;
    }

  return true;
}

bool test1_dynarray_find(void *obj_to_find, int size_of_dynarray)
{
  dynarray *new_array;
  void* str_returned;
  test_struct* tst3;
  
  new_array = dynarray_create(1, (void*)&test_struct1, sizeof(test_struct1), compare);

  if (dynarray_add(new_array, (void*)&test_struct1) == false)
    {
      return false;
    }


  
  if ((str_returned = dynarray_find(new_array, obj_to_find)) == NULL)
    {
      return false;
    }

  tst3 = (test_struct*)str_returned;

  if (tst3->integer != test_struct1.integer ||
      tst3->floating_float != test_struct1.floating_float ||
      tst3->double_test != test_struct1.double_test)
    {
      return false;
    }
  
  return true;
}

void output_test_results(bool return_value, char* msg)
{
  static int test_nb = 0;

  if (return_value == true)
   printf("%s.%d: succeeded!\n", msg, test_nb);
  else
    printf("%s.%d: failed.\n", msg, test_nb);

  test_nb++;
}

void crash_functions(void)
{
  output_test_results(test1_dynarray_add(5, 6), "dynarray_add()");
}

int main(void)
{

  // create  
  output_test_results(!test1_dynarray_create(1, 0), "dynarray_create()");
  output_test_results(test1_dynarray_create(1, 5), "dynarray_create()");
  output_test_results(!test1_dynarray_create(50, 0), "dynarray_create()");
  output_test_results(test1_dynarray_create(50, 5), "dynarray_create()");

  // add
  output_test_results(test1_dynarray_add(5, 5), "dynarray_add()");
  output_test_results(!test1_dynarray_add(0, 0), "dynarray_add()");
  output_test_results(!test1_dynarray_add(0, 1), "dynarray_add()");  

  // search
  output_test_results(test1_dynarray_search((void*)&test_struct1, 1), "dynarray_search()");
  output_test_results(test1_dynarray_search((void*)&test_struct1, 0), "dynarray_search()");
  output_test_results(!test1_dynarray_search((void*)&test_struct2, 1), "dynarray_search()");
    
  // find
  output_test_results(test1_dynarray_find((void*)&test_struct1, 1), "dynarray_find()");
  output_test_results(test1_dynarray_find((void*)&test_struct1, 0), "dynarray_find()");
  output_test_results(!test1_dynarray_find((void*)&test_struct2, 1), "dynarray_find()");

  // remove
  output_test_results(test1_dynarray_remove(1, 0), "dynarray_remove()");
  output_test_results(!test1_dynarray_remove(1, -1), "dynarray_remove()");
  output_test_results(!test1_dynarray_remove(1, 1), "dynarray_remove()");

  // element
  output_test_results(!test1_dynarray_element(0, 1), "dynarray_element()");
  output_test_results(!test1_dynarray_element(1, 1), "dynarray_element()");
  output_test_results(test1_dynarray_element(1, 0), "dynarray_element()");
    
  //isempty isfull count free
  dynarray* new_array = dynarray_create(5, (void*)&test_struct1, sizeof(test_struct1), compare);

  dynarray_isempty(new_array);
  dynarray_isfull(new_array);
  dynarray_count(new_array);
  dynarray_free(new_array);
  
  //crash_functions();
 
  return 0;
}
