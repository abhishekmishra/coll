#include <limits.h>
#include "coll_arraylist.h"
#include <stdio.h>

int arraylist_new(arraylist **l, arraylist_free_function *free_fn)
{
	return arraylist_new_with_capacity(l, 0, free_fn);
}

int arraylist_new_with_capacity(arraylist **l, size_t capacity, arraylist_free_function *free_fn)
{
	(*l) = (arraylist *)calloc(1, sizeof(arraylist));
	if ((*l) == NULL)
	{
		return E_ARRAYLIST_UNABLE_TO_ALLOCATE_ARRAY;
	}

	arraylist *list = (*l);

	// set the capacity of the newly created list
	if (capacity >= 0 && capacity <= MAX_VOIDPTR_ALLOCATE)
	{
		if (capacity == 0)
		{
			list->capacity = ARRAYLIST_DEFAULT_SIZE;
		}
		else
		{
			list->capacity = capacity;
		}
	}

	// set the size to 0
	list->size = 0;

	// set the free function
	list->free_fn = free_fn;

	// allocate the array for the items
	list->array = (void **)calloc(sizeof(void *), list->capacity);
	if (list->array == NULL)
	{
		free(list);
		return E_ARRAYLIST_UNABLE_TO_ALLOCATE_ARRAY;
	}

#ifdef APR_ENABLED
	list->pool = NULL;
#endif //APR_ENABLED

	return 0;
}

#ifdef APR_ENABLED

int arraylist_apr_new(arraylist **l, apr_pool_t *pool, arraylist_free_function *free_fn)
{
	return arraylist_apr_new_with_capacity(l, pool, 0, free_fn);
}

int arraylist_apr_new_with_capacity(arraylist **l, apr_pool_t *pool, size_t capacity, arraylist_free_function *free_fn)
{
	if (pool == NULL)
	{
		return E_ARRAYLIST_UNABLE_TO_ALLOCATE_ARRAY;
	}

	(*l) = (arraylist *)apr_palloc(pool, 1 * sizeof(arraylist));
	if ((*l) == NULL)
	{
		return E_ARRAYLIST_UNABLE_TO_ALLOCATE_ARRAY;
	}

	arraylist *list = (*l);

	// set the capacity of the newly created list
	if (capacity >= 0 && capacity <= MAX_VOIDPTR_ALLOCATE)
	{
		if (capacity == 0)
		{
			list->capacity = ARRAYLIST_DEFAULT_SIZE;
		}
		else
		{
			list->capacity = capacity;
		}
	}

	// set the size to 0
	list->size = 0;

	// set the free function
	list->free_fn = free_fn;

	// allocate the array for the items
	list->array = (void **)apr_pcalloc(pool, sizeof(void *) * list->capacity);
	if (list->array == NULL)
	{
		free(list);
		return E_ARRAYLIST_UNABLE_TO_ALLOCATE_ARRAY;
	}

	list->pool = pool;

	return 0;
}

#endif // APR_ENABLED

size_t arraylist_length(arraylist *l)
{
	if (l == NULL)
		return 0;
	return l->size;
}

int arraylist_resize(arraylist *l, size_t max)
{
	void **new_array;
	size_t new_capacity;

	// if the size of the list is less than the max, then don't resize
	if (max < l->capacity)
	{
		return 0;
	}

	// if current capacity is less than half of MAX_VOIDPTR_ALLOCATE
	// try to increase the capacity by 50%
	if (l->capacity < MAX_VOIDPTR_ALLOCATE / 2)
	{
		// increase capacity
		size_t capacity_to_add = (l->capacity / 2) == 0 ? 1 : (l->capacity / 2);
		new_capacity = l->capacity + capacity_to_add;

		// if capacity is still less than max
		if (new_capacity < max)
		{
			new_capacity = max;
		}
	}
	else
	{
		// no choice but to increment to the max.
		new_capacity = max;
	}

	// if the new_capacity is more than the total number
	// of void* that can be contiguously allocated
	if (new_capacity > MAX_VOIDPTR_ALLOCATE)
	{
		return E_ARRAYLIST_UNABLE_TO_ALLOCATE_ARRAY;
	}
	else
	{
		// reallocate array
		printf("Expanding capacity to %zu\n", new_capacity);
#ifdef APR_ENABLED
		if (l->pool == NULL)
		{
#endif // APR_ENABLED
			new_array = (void **)realloc(l->array, (new_capacity * sizeof(void *)));
			if (new_array == NULL)
			{
				return E_ARRAYLIST_UNABLE_TO_ALLOCATE_ARRAY;
			}
			l->array = new_array;
			l->capacity = new_capacity;
#ifdef APR_ENABLED
		}
		else
		{
			new_array = (void **)apr_pcalloc(l->pool, (new_capacity * sizeof(void *)));
			if (new_array == NULL)
			{
				return E_ARRAYLIST_UNABLE_TO_ALLOCATE_ARRAY;
			}
			memcpy(new_array, l->array, l->capacity * sizeof(void *));
			l->array = new_array;
			l->capacity = new_capacity;
			// old array need not be freed as it will be removed
			// when pool is destroyed
		}
#endif // APR_ENABLED
		return 0;
	}
}

int arraylist_insert(arraylist *l, size_t loc, void *item)
{
	if (loc > l->size)
	{
		return E_ARRAYLIST_INDEX_NOT_FOUND;
	}
	if (loc > MAX_VOIDPTR_ALLOCATE - 1)
	{
		return E_ARRAYLIST_INDEX_BEYOND_CAPACITY;
	}
	if (arraylist_resize(l, loc))
	{
		return E_ARRAYLIST_UNABLE_TO_ALLOCATE_ARRAY;
	}
	if (loc < l->size)
	{
		// need to move all items ahead by one index
		for (size_t i = l->size; i > loc; i--)
		{
			l->array[i] = l->array[i - 1];
		}
	}
	l->array[loc] = item;
	l->size = l->size + 1;
	return 0;
}

int arraylist_add(arraylist *l, void *item)
{
	return arraylist_insert(l, arraylist_length(l), item);
}

int arraylist_set(arraylist *l, size_t loc, void *item)
{
	if (loc > MAX_VOIDPTR_ALLOCATE - 1)
	{
		return E_ARRAYLIST_INDEX_BEYOND_CAPACITY;
	}
	if (loc < l->size)
	{
		if (l->free_fn != NULL)
		{
			l->free_fn(l->array[loc]);
		}
		l->array[loc] = item;
		return 0;
	}
	else
	{
		return E_ARRAYLIST_INDEX_NOT_FOUND;
	}
}

void *arraylist_get(arraylist *l, size_t loc)
{
	if (loc < l->size)
	{
		return l->array[loc];
	}
	else
	{
		return NULL;
	}
}

void arraylist_clear(arraylist *l)
{
	if (l->free_fn != NULL)
	{
		for (size_t i = 0; i < l->size; i++)
		{
			l->free_fn(l->array[i]);
		}
	}
	l->size = 0;
}

int arraylist_delete(arraylist *l, size_t loc)
{
	if (loc < l->size)
	{
		if (l->free_fn != NULL)
		{
			l->free_fn(l->array[loc]);
		}
		for (size_t i = loc; i < l->size - 1; i++)
		{
			l->array[i] = l->array[i + 1];
		}
		l->size = l->size - 1;
		return 0;
	}
	else
	{
		return E_ARRAYLIST_INDEX_NOT_FOUND;
	}
}

void arraylist_free(arraylist *l)
{
	if (l != NULL)
	{
		if (l->free_fn != NULL)
		{
			for (size_t i = 0; i < l->size; i++)
			{
				l->free_fn(l->array[i]);
			}
		}
		free(l->array);
		free(l);
	}
}

#ifdef APR_ENABLED

void arraylist_apr_free(arraylist* l)
{
	if (l != NULL)
	{
		if (l->free_fn != NULL)
		{
			for (size_t i = 0; i < l->size; i++)
			{
				l->free_fn(l->array[i]);
			}
		}
	}
}

#endif //APR_ENABLED

void arraylist_print(arraylist *l, void (*item_print)(void *item))
{
	printf("ArrayList size=%zu, capacity=%zu\n[", l->size, l->capacity);
	size_t i;
	for (i = 0; i < l->size - 1; i++)
	{
		item_print(l->array[i]);
		printf(", ");
	}
	item_print(l->array[i]);
	printf("]\n");
}