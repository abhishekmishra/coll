#include <stdio.h>
#include <stdlib.h>

#include "test_arraylist.h"
#include "test_lualib_arraylist.h"

#ifdef LUA_ENABLED
#include "test_coll_arraylist_map.h"
#endif

int main(int argc, char **argv) {
    int res;
	printf("==========================RUNNING ARRAYLIST TESTS==========================\n");
    res = arraylist_tests();
	printf("===========================DONE ARRAYLIST TESTS============================\n");
#ifdef LUA_ENABLED
	printf("==========================RUNNING LUA ARRAYLIST TESTS==========================\n");
	res = lualib_arraylist_tests();
	printf("===========================DONE LUA ARRAYLIST TESTS============================\n");
#endif
	printf("==========================RUNNING ARRAYLIST MAP TESTS==========================\n");
	res = coll_al_map_tests();
	printf("===========================DONE ARRAYLIST MAP TESTS============================\n");

	return res;
}