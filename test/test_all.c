#include <stdio.h>
#include <stdlib.h>

#include "test_arraylist.h"
#include "test_lualib_arraylist.h"
#include "test_coll_arraylist_map.h"

int main(int argc, char **argv) {
    int res;
	//printf("==========================RUNNING ARRAYLIST TESTS==========================\n");
 //   res = arraylist_tests();
	//printf("===========================DONE ARRAYLIST TESTS============================\n");
	//printf("==========================RUNNING LUA ARRAYLIST TESTS==========================\n");
	//res = lualib_arraylist_tests();
	//printf("===========================DONE LUA ARRAYLIST TESTS============================\n");
	printf("==========================RUNNING ARRAYLIST MAP TESTS==========================\n");
	res = coll_al_map_tests();
	printf("===========================DONE ARRAYLIST MAP TESTS============================\n");

	return res;
}