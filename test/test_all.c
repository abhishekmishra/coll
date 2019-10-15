#include <stdio.h>
#include <stdlib.h>

#include "test_arraylist.h"

int main(int argc, char **argv) {
    int res;
	printf("==========================RUNNING ARRAYLIST TESTS==========================\n");
    res = arraylist_tests();
	printf("===========================DONE ARRAYLIST TESTS============================\n");
    return res;
}