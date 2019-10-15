#ifndef ___ARRAYLIST_H___
#define ___ARRAYLIST_H___

#include <stdlib.h>

/**
* ArrayList: This is a datastructure which implements the
* abstract datatype (ADT) list, using a fixed length array.
* The internal array is resized when it reaches capacity, and
* it's capacity is increased.
* 
* A list has the following operations:
* 1. Insert/Put at location l
* 1.1 Push/Add at the end of the list
* 2. Locate/Find item with value x
* 3. Retrieve/Get value at location l
* 4. Delete the value at location l
* 5. Next - get the next location given location l
* 6. Previous - get the previous location given location l
* (Next and Previous are trivial indexing operations in an arraylist)
* 7. Makenull - empty the list
* 8. First - gets the first location of the list (this is always 0 in an arraylist)
* 9. PrintList - prints the list to the console/ to a string.
* 10. Free - destructor/cleanup the list.
**/

#define ARRAYLIST_DEFAULT_SIZE 50

#define E_ARRAYLIST_UNABLE_TO_ALLOCATE_ARRAY 1
#define E_ARRAYLIST_INDEX_BEYOND_CAPACITY 2

typedef void (arraylist_free_function)(void* data);

/**
* This struct represents the ArrayList and it's pointer is
* the user's handle to the arraylist.
* 
* The values in the struct should be considered readonly,
* and should not be modified by user of the arraylist.
* User should always use the public methods of this implementation.
*
* The capacity member of the struct shows the maximum number of items
* this arraylist can hold. This number should be carefully chosen at
* the time of creation of the arraylist.
*
* The size holds the count of items currently in the list.
*
* The array member is a pointer to an array of void pointers, each of
* which point to one value. Each value inserted in the array must be allocated
* by the caller and the function to free the items must be passed to the constructor.
**/
typedef struct arraylist_t {
	void** array;
	size_t capacity;
	size_t size;
	arraylist_free_function* free_fn;
} arraylist;

/**
* Create a new arraylist with the default initial capacity.
*
* @param l an unitialized arraylist pointer, which will be created by the constructor.
* @param free the function used to free the items in the arraylist
* @return value indicating success or falilure (0 is success)
**/
extern int arraylist_new(arraylist** l, arraylist_free_function* free_fn);

/**
* Create a new arraylist with the specified initial capacity.
* 
* @param l an unitialized arraylist pointer, which will be created by the constructor.
* @param capacity total capacity of the arraylist (0 will set capacity to default)
* @param free the function used to free the items in the arraylist
* @return value indicating success or falilure (0 is success)
**/
extern int arraylist_new_with_capacity(arraylist** l, size_t capacity, arraylist_free_function* free_fn);

/**
* Insert item at location loc of the arraylist
* 
* @param l the arraylist
* @param loc location to insert at
* @param item item to insert
* @return error code
**/
int arraylist_insert(arraylist* l, size_t loc, void* item);




#endif /* ___ARRAYLIST_H___ */