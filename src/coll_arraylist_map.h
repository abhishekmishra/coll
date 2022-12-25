#ifndef ___COLL_AL_MAP_H___
#define ___COLL_AL_MAP_H___

#ifdef __cplusplus  
extern "C" {
#endif

/**
 * @file coll_arraylist_map.h
* AL_MAP: This is a datastructure which provides
* a naive implementation for a Map/Dictionary, backed
* by the ArrayList datastructure implemented in the same
* library.
*
*
**/

#include "coll_arraylist.h"
#include <stdbool.h>

#define COLL_AL_MAP_DEFAULT_CAPACITY 25
#define E_COLL_AL_MAP_ALLOC_FAILED 10
#define E_COLL_AL_MAP_NULL 11
#define E_COLL_AL_MAP_ALREADY_EXISTS 12
#define E_COLL_AL_MAP_KEY_NULL 13
#define E_COLL_AL_MAP_COMPARE_FN_NULL 15
#define E_COLL_AL_MAP_SUCCESS 0

typedef int (coll_al_map_compare_fn) (void* first, void* second);

typedef void (coll_al_map_iter_fn)(size_t index, void* key, void* value);

typedef struct coll_al_map_t {
    coll_al_map_compare_fn* compare;
    size_t size;
    arraylist* keys;
    arraylist* values;
} coll_al_map;

#define coll_al_map_keys(map)				map->keys
#define coll_al_map_keys_length(map)		arraylist_length(coll_al_map_keys(map))
#define coll_al_map_keys_get_idx(map, i)	arraylist_get(coll_al_map_keys(map), i)
#define coll_al_map_values(map)				map->values
#define coll_al_map_values_length(map)		arraylist_length(coll_al_map_values(map))
#define coll_al_map_values_get_idx(map, i)	arraylist_get(coll_al_map_values(map), i)

/**
 * Create a new map based on arraylist,
 * with default initial capacity.
 * 
 * Returns NULL if unable to allocate the map.
 * 
 * @param compare function to compare two keys
 * @return arraylist map
 */
coll_al_map* make_coll_al_map(coll_al_map_compare_fn* compare);

/**
 * Create a new map based on arraylist,
 * with given initial capacity.
 * 
 * Returns NULL if unable to allocate the map.
 * 
 * @param compare function to compare two keys
 * @param initial_capacity initial capacity of the keys and values lists
 * @return arraylist map
 */
coll_al_map* make_coll_al_map_w_capacity(coll_al_map_compare_fn* compare, size_t initial_capacity);

/**
 * Put a new key value pair in the map.
 * Returns E_COLL_AL_MAP_ALREADY_EXISTS if the key exists.
 * Returns E_COLL_AL_MAP_KEY_NULL if key is NULL.
 * Returns E_COLL_AL_MAP_ALLOC_FAILED if allocation failed.
 * Returns E_COLL_AL_MAP_SUCCESS (0) on success.
 * 
 * @param map the map to insert the value.
 * @param key the key
 * @param value the value
 * @return error code
 */
int coll_al_map_put(coll_al_map* map, void* key, void* val);

/**
 * Get the value associated with the key in the map
 * Returns NULL if value is NULL or not found.
 * 
 * @param map the map
 * @param key the key
 * @return the value
 */
void* coll_al_map_get(coll_al_map* map, void* key);

/**
 * Remove the key/value pair identified by the key.
 * The iter_fn if provided is called with index, key and value,
 * just before the delete. This allows the caller to perform an
 * operation such as free on the items being deleted.
 *
 * The items are not freed when they are deleted from the arraylist.
 *
 * @param map the map
 * @param key the key
 * @return flag indicating whether key is found.
 */
bool coll_al_map_remove(coll_al_map* map, void* key, coll_al_map_iter_fn* iter_fn);

/**
 * Return a flag indicating whether the given key is in the map.
 * 
 * @param map the map
 * @param key the key
 * @return flag indicating whether key is found.
 */
bool coll_al_map_has(coll_al_map* map, void* key);

/**
 * Free the map. The keys and values should be freed
 * before freeing the map.
 * 
 * @param map the map
 */
void free_coll_al_map(coll_al_map* map);

/**
 * Iterate over the key/value pairs of the map.
 *
 * @param map the map
 * @param iter_fn the function which is called per index, key, value
 */
void coll_al_map_foreach_fn(coll_al_map* map, coll_al_map_iter_fn* iter_fn);

#ifdef __cplusplus 
}
#endif

#endif /* ___COLL_AL_MAP_H___ */
