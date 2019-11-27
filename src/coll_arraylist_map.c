#include <stdlib.h>
#include "coll_arraylist_map.h"

coll_al_map* make_coll_al_map(coll_al_map_compare_fn* compare) {
	return make_coll_al_map_w_capacity(compare, COLL_AL_MAP_DEFAULT_CAPACITY);
}

coll_al_map* make_coll_al_map_w_capacity(coll_al_map_compare_fn* compare, size_t initial_capacity) {
	coll_al_map* map = (coll_al_map*)calloc(1, sizeof(coll_al_map));
	if (map == NULL) {
		return NULL;
	}

	map->size = 0;
	if (compare == NULL) {
		return NULL;
	}
	map->compare = compare;

	int err = arraylist_new_with_capacity(&map->keys, initial_capacity, NULL);
	if (err != 0) {
		return NULL;
	}

	err = arraylist_new_with_capacity(&map->values, initial_capacity, NULL);
	if (err != 0) {
		return NULL;
	}

	return map;
}

int coll_al_map_put(coll_al_map* map, void* key, void* val) {
	if (map == NULL) {
		return E_COLL_AL_MAP_NULL;
	}
	if (key == NULL) {
		return E_COLL_AL_MAP_KEY_NULL;
	}
	if (coll_al_map_has(map, key)) {
		return E_COLL_AL_MAP_ALREADY_EXISTS;
	}
	int err = arraylist_add(map->keys, key);
	if (err > 0) {
		return E_COLL_AL_MAP_ALLOC_FAILED;
	}
	err = arraylist_add(map->values, val);
	if (err > 0) {
		arraylist_delete(map->keys, arraylist_length(map->keys) - 1);
		return E_COLL_AL_MAP_ALLOC_FAILED;
	}
	map->size = arraylist_length(map->keys);
	return E_COLL_AL_MAP_SUCCESS;
}

void* coll_al_map_get(coll_al_map* map, void* key) {
	if (map == NULL) {
		return NULL;
	}
	if (key == NULL) {
		return NULL;
	}

	bool found = false;
	size_t loc;
	for (size_t i = 0; i < arraylist_length(map->keys); i++) {
		void* k = arraylist_get(map->keys, i);
		if (map->compare(key, k) == 0) {
			found = true;
			loc = i;
		}
	}
	if (found) {
		return arraylist_get(map->values, loc);
	}
	return NULL;
}

bool coll_al_map_remove(coll_al_map* map, void* key, coll_al_map_iter_fn* iter_fn) {
	bool found = false;
	size_t loc;
	for (size_t i = 0; i < arraylist_length(map->keys); i++) {
		void* k = arraylist_get(map->keys, i);
		if (map->compare(key, k) == 0) {
			found = true;
			loc = i;
		}
	}
	if (found) {
		if (iter_fn != NULL) {
			iter_fn(loc, arraylist_get(map->keys, loc), arraylist_get(map->values, loc));
		}
		arraylist_delete(map->keys, loc);
		arraylist_delete(map->values, loc);
		map->size = arraylist_length(map->keys);
	}
}

bool coll_al_map_has(coll_al_map* map, void* key) {
	for (size_t i = 0; i < arraylist_length(map->keys); i++) {
		void* k = arraylist_get(map->keys, i);
		if (map->compare(key, k) == 0) {
			return true;
		}
	}
	return false;
}

void free_coll_al_map(coll_al_map* map) {
	if (map != NULL) {
		arraylist_free(map->keys);
		arraylist_free(map->values);
		free(map);
	}
}

void coll_al_map_foreach(coll_al_map* map, coll_al_map_iter_fn* iter_fn) {
	if (map == NULL) {
		return;
	}
	if (iter_fn != NULL) {
		for (size_t i = 0; i < map->size; i++) {
			iter_fn(i, arraylist_get(map->keys, i), arraylist_get(map->values, i));
		}
	}
}