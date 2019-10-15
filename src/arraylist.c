#include <limits.h>
#include "arraylist.h"

int arraylist_new(arraylist** l, arraylist_free_function* free_fn) {
	return arraylist_new_with_capacity(l, 0, free_fn);
}

int arraylist_new_with_capacity(arraylist** l, size_t capacity, arraylist_free_function* free_fn) {
	(*l) = (arraylist*)calloc(1, sizeof(arraylist));
	if ((*l) == NULL) {
		return E_ARRAYLIST_UNABLE_TO_ALLOCATE_ARRAY;
	}

	arraylist* list = (*l);
	
	// set the capacity of the newly created list
	if (capacity >= 0 && capacity <= SIZE_MAX) {
		if (capacity == 0) {
			list->capacity = ARRAYLIST_DEFAULT_SIZE;
		}
		else {
			list->capacity = capacity;
		}
	}

	// set the size to 0
	list->size = 0;

	// set the free function
	list->free_fn = free_fn;

	// allocate the array for the items
	list->array = (void**)calloc(sizeof(void*), list->capacity);
	if (list->array == NULL) {
		free(list);
		return E_ARRAYLIST_UNABLE_TO_ALLOCATE_ARRAY;
	}

	return 0;
}

int arraylist_resize(arraylist* l, size_t max) {
	size_t new_capacity;

	// if the size of the list is less than the max, then don't resize
	if (max < l->size) {
		return 0;
	}

	// if current capacity is less than half of SIZE_MAX
	// try to double the capacity, so that we have to do
	// fewer reallocations.
	// TODO: However this also means that reallocations will proceed
	// with size of 1 after array size has reached SIZE_MAX/2

	if (l->capacity < SIZE_MAX / 2) {
		//double capacity
		new_capacity = l->capacity * 2;

		// if capacity is still less than max
		if (new_capacity < max) {
			new_capacity = max;
		}
	}
	else {
		//no choice but to increment to the max.
		new_capacity = max;
	}

	// if the new_capacity is 
}

int arraylist_insert(arraylist* l, size_t loc, void* item) {
	if (loc > SIZE_MAX - 1) {
		return E_ARRAYLIST_INDEX_BEYOND_CAPACITY;
	}
}