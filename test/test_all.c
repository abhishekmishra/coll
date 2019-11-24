#include <stdio.h>
#include <stdlib.h>

#include "test_arraylist.h"
#include "test_lualib_arraylist.h"

int main(int argc, char **argv) {
    int res;
	printf("==========================RUNNING ARRAYLIST TESTS==========================\n");
    res = arraylist_tests();
	printf("===========================DONE ARRAYLIST TESTS============================\n");
	printf("==========================RUNNING LUA ARRAYLIST TESTS==========================\n");
	res = lualib_arraylist_tests();
	printf("===========================DONE LUA ARRAYLIST TESTS============================\n");
	return res;
}