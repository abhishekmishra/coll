#include <limits.h>
#include "arraylist.h"
#include <stdio.h>

#define MAX_VOIDPTR_ALLOCATE (SIZE_MAX / sizeof(void*))

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

size_t arraylist_length(arraylist* l) {
	if (l == NULL) return NULL;
	return l->size;
}

int arraylist_resize(arraylist* l, size_t max) {
	void** new_array;
	size_t new_capacity;

	// if the size of the list is less than the max, then don't resize
	if (max < l->capacity) {
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

	// if the new_capacity is more than the total number
	// of void* that can be contiguously allocated
	if (new_capacity > MAX_VOIDPTR_ALLOCATE) {
		return E_ARRAYLIST_UNABLE_TO_ALLOCATE_ARRAY;
	}
	else {
		//reallocate array
		printf("Expanding capacity to %zu\n", new_capacity);
		new_array = (void**)realloc(l->array, (new_capacity * sizeof(void*)));
		if (new_array == NULL) {
			return E_ARRAYLIST_UNABLE_TO_ALLOCATE_ARRAY;
		}
		l->array = new_array;
		l->capacity = new_capacity;
		return 0;
	}
}

int arraylist_insert(arraylist* l, size_t loc, void* item) {
	if (loc > SIZE_MAX - 1) {
		return E_ARRAYLIST_INDEX_BEYOND_CAPACITY;
	}
	if (arraylist_resize(l, loc)) {
		return E_ARRAYLIST_UNABLE_TO_ALLOCATE_ARRAY;
	}
	if (loc < l->size) {
		//need to move all items ahead by one index
		for (size_t i = l->size; i > loc; i--) {
			l->array[i] = l->array[i - 1];
		}
	}
	l->array[loc] = item;
	l->size = l->size + 1;
}

int arraylist_add(arraylist* l, void* item) {
	return arraylist_insert(l, arraylist_length(l), item);
}

int arraylist_set(arraylist* l, size_t loc, void* item) {
	if (loc > SIZE_MAX - 1) {
		return E_ARRAYLIST_INDEX_BEYOND_CAPACITY;
	}
	if (loc < l->size) {
		if (l->free_fn != NULL) {
			l->free_fn(l->array[loc]);
		}
		l->array[loc] = item;
		l->size = l->size + 1;
	}
	else {
		return E_ARRAYLIST_INDEX_NOT_FOUND;
	}
}

void* arraylist_get(arraylist* l, size_t loc) {
	if (loc < l->size) {
		return l->array[loc];
	}
	else {
		return NULL;
	}
}

void arraylist_clear(arraylist* l) {
	for (size_t i = 0; i < l->size; i++) {
		l->free_fn(l->array[i]);
	}
	l->size = 0;
}

void arraylist_print(arraylist* l, void (*item_print)(void* item)) {
	printf("ArrayList size=%zu, capacity=%zu\n[", l->size, l->capacity);
	for (size_t i = 0; i < l->size; i++) {
		item_print(l->array[i]);
		if (i != l->size - 1) {
			printf(", ");
		}
	}
	printf("]\n");
}