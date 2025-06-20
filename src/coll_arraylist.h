#ifndef ___ARRAYLIST_H___
#define ___ARRAYLIST_H___

#ifdef __cplusplus  
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>

#ifdef LUA_ENABLED

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#endif //LUA_ENABLED

#ifdef APR_ENABLED

#include <apr.h>
#include <apr_pools.h>

#endif //APR_ENABLED

/**
 * @file coll_arraylist.h
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
#define E_ARRAYLIST_INDEX_NOT_FOUND 3

#define MAX_VOIDPTR_ALLOCATE (SIZE_MAX / sizeof(void*))

typedef void (arraylist_free_function)(void* data);

#ifdef LUA_ENABLED
typedef void (arraylist_item_to_lua_object)(lua_State* L, int index, void* data);
#endif //LUA_ENABLED

/**
* This struct represents the ArrayList and its pointer is
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
	size_t capacity;
	size_t size;
	arraylist_free_function* free_fn;
#ifdef LUA_ENABLED
	arraylist_item_to_lua_object* convert_to_lua;
#endif //LUA_ENABLED
#ifdef APR_ENABLED
	apr_pool_t* pool;
#endif //APR_ENABLED
	void** array;
} arraylist;

/**
* Create a new arraylist with the default initial capacity.
*
* @param l an uninitialized arraylist pointer, which will be created by the constructor.
* @param free the function used to free the items in the arraylist
* @return value indicating success or failure (0 is success)
**/
int arraylist_new(arraylist** l, arraylist_free_function* free_fn);

/**
* Create a new arraylist with the specified initial capacity.
* 
* @param l an uninitialized arraylist pointer, which will be created by the constructor.
* @param capacity total capacity of the arraylist (0 will set capacity to default)
* @param free the function used to free the items in the arraylist
* @return value indicating success or falilure (0 is success)
**/
int arraylist_new_with_capacity(arraylist** l, size_t capacity, arraylist_free_function* free_fn);

#ifdef LUA_ENABLED

/**
 * Set the function which converts items in this arraylist to lua objects.
 *
 * @param l the arraylist
 * @param convert_to_lua the conversion function.
 **/
void set_lua_convertor(arraylist* l, arraylist_item_to_lua_object* convert_to_lua);

/**
 * Convert the given arraylist to a lua array.
 * The lua array will be a read-only representation
 * of the arraylist.
 * @param list the arraylist
 * @param L the lua_State
 */
void convert_to_lua_array(arraylist* list, lua_State* L);

#endif //LUA_ENABLED

/**
* Return the size/length of the arraylist
*
* @param l the arraylist
* @return length/size of the array.
**/
size_t arraylist_length(arraylist* l);

/**
* Insert item at location loc of the arraylist.
* Can insert at any location inclusive of 0 to 
* length of array (that is one beyond the last element).
* 
* If loc is greater than current size, then 
* returns E_ARRAYLIST_INDEX_NOT_FOUND.
* If the size of current arraylist has already reached MAX_VOIDPTR_ALLOCATE,
* then returns E_ARRAYLIST_INDEX_BEYOND_CAPACITY.
* If reallocation of arraylist to accomodate new item fails,
* then returns E_ARRAYLIST_UNABLE_TO_ALLOCATE_ARRAY.
* 
* @param l the arraylist
* @param loc location to insert at
* @param item item to insert
* @return error code
**/
int arraylist_insert(arraylist* l, size_t loc, void* item);

/**
* Insert an item at the end of the arraylist
* equivalent to arraylist_insert(l, arraylist_length(l), item)
* If the size of current arraylist has already reached MAX_VOIDPTR_ALLOCATE,
* then returns E_ARRAYLIST_INDEX_BEYOND_CAPACITY.
* If reallocation of arraylist to accomodate new item fails,
* then returns E_ARRAYLIST_UNABLE_TO_ALLOCATE_ARRAY.
*
* @param l the arraylist
* @param item item to insert
* @return error code
**/
int arraylist_add(arraylist* l, void* item);

/**
* Set the item at location loc of the arraylist.
* If the location specified is greater than MAX_VOIDPTR_ALLOCATE, then
* an error code E_ARRAYLIST_INDEX_BEYOND_CAPACITY is returned.
* If loc is greater than current length of the arraylist,
* then E_ARRAYLIST_INDEX_NOT_FOUND is returned.
*
* @param l the arraylist
* @param loc location to insert at
* @param item item to insert
* @return error code
**/
int arraylist_set(arraylist* l, size_t loc, void* item);

/**
* Get the item at location loc of the arraylist.
* Returns NULL if the location is beyond the length of the arraylist
*
* @param l the arraylist
* @param loc location to insert at
* @return item
**/
void* arraylist_get(arraylist* l, size_t loc);

/**
* Clear the array of all elements, but do not de-allocate.
* 
* @param l the arraylist
**/
void arraylist_clear(arraylist* l);

/**
* Deletes the item at location loc of the arraylist.
* If a `free_fn` was provided during list creation, the item at `loc`
* will be freed using that function. Otherwise, the pointer is removed
* from the list without freeing the underlying item (user remains responsible).
*
* @param l the arraylist.
* @param loc location to delete at.
* @return 0 on success, or an error code (e.g., E_ARRAYLIST_INDEX_NOT_FOUND) if
*         the location is invalid. This function does not return the deleted item.
**/
int arraylist_delete(arraylist* l, size_t loc);

/** 
* Free the arraylist.
* (Calls free_fn to free existing items if provided at creation).
*
* @param l the arraylist
**/
void arraylist_free(arraylist* l);

//TODO: reconsider function to return string, instead of print
/**
* Print the arraylist on the console.
* @param l the arraylist
* @param item_print fn pointer which prints a single item
* @return void
**/
void arraylist_print(arraylist* l, void (*item_print)(void* item));

#ifdef APR_ENABLED

int arraylist_apr_new(arraylist** l, apr_pool_t* pool, arraylist_free_function* free_fn);
int arraylist_apr_new_with_capacity(arraylist** l, apr_pool_t* pool, size_t capacity, arraylist_free_function* free_fn);
void arraylist_apr_free(arraylist* l);

#endif //APR_ENABLED

#ifdef __cplusplus 
}
#endif

#endif /* ___ARRAYLIST_H___ */
