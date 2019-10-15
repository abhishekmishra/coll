#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <stdio.h>

#include "test_arraylist.h"
#include "arraylist.h"

static arraylist* intptrlist0;

void free_intptr(int* p) {
	free(p);
}

void print_intptr(int* p) {
	printf("%d", *p);
}

static int group_setup(void** state) {
	int err = arraylist_new(&intptrlist0, &free_intptr);
	return err;
}
static int group_teardown(void** state) {
	//TODO free the arraylist once function is implemented.
	return 0;
}

static void test_arraylist_new_create_only(void** state) {
	arraylist* test_l;
	int err = arraylist_new(&test_l, NULL);
	assert_int_equal(err, 0);
}

static void test_arraylist_insert_at_beginning(void** state) {
	for (int i = 0; i < 5; i++) {
		int* x = (int*)calloc(1, sizeof(int));
		assert_non_null(x);
		if (x != NULL) {
			*x = i;
			arraylist_insert(intptrlist0, 0, x);
		}
	}
	arraylist_print(intptrlist0, print_intptr);
	for (int i = 0; i < 5; i++) {
		assert_int_equal(*((int*)(arraylist_get(intptrlist0, i))), 4ULL-i);
	}
	arraylist_clear(intptrlist0);
}

static void test_arraylist_insert_at_end(void** state) {
	for (int i = 0; i < 5; i++) {
		int* x = (int*)calloc(1, sizeof(int));
		assert_non_null(x);
		if (x != NULL) {
			*x = i;
			arraylist_insert(intptrlist0, arraylist_length(intptrlist0), x);
		}
	}
	arraylist_print(intptrlist0, print_intptr);
	for (int i = 0; i < 5; i++) {
		assert_int_equal(*((int*)(arraylist_get(intptrlist0, i))), i);
	}
	arraylist_clear(intptrlist0);
}

static void test_arraylist_insert_beyond_capacity(void** state) {
	for (int i = 0; i < 101; i++) {
		int* x = (int*)calloc(1, sizeof(int));
		assert_non_null(x);
		if (x != NULL) {
			*x = i;
			arraylist_insert(intptrlist0, arraylist_length(intptrlist0), x);
		}
	}
	//arraylist_print(intptrlist0, print_intptr);
	for (int i = 0; i < 100; i++) {
		assert_int_equal(*((int*)(arraylist_get(intptrlist0, i))), i);
	}
	arraylist_clear(intptrlist0);
}

static void test_arraylist_insert_large(void** state) {
	for (size_t i = 0; i < 65535; i++) {
		int* x = (int*)calloc(1, sizeof(int));
		assert_non_null(x);
		if (x != NULL) {
			*x = (int)i;
			arraylist_insert(intptrlist0, arraylist_length(intptrlist0), x);
		}
	}
	//arraylist_print(intptrlist0, print_intptr);
	for (int i = 0; i < 100; i++) {
		assert_int_equal(*((int*)arraylist_get(intptrlist0, i)), i);
	}
	arraylist_clear(intptrlist0);
}

static void test_arraylist_add(void** state) {
	for (int i = 0; i < 5; i++) {
		int* x = (int*)calloc(1, sizeof(int));
		assert_non_null(x);
		if (x != NULL) {
			*x = i;
			arraylist_add(intptrlist0, x);
		}
	}
	arraylist_print(intptrlist0, print_intptr);
	for (int i = 0; i < 5; i++) {
		assert_int_equal(*((int*)(arraylist_get(intptrlist0, i))), i);
	}
	arraylist_clear(intptrlist0);
}


int arraylist_tests() {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_arraylist_new_create_only),
		cmocka_unit_test(test_arraylist_insert_at_beginning),
		cmocka_unit_test(test_arraylist_insert_at_end),
		cmocka_unit_test(test_arraylist_insert_beyond_capacity),
		cmocka_unit_test(test_arraylist_insert_large),
		cmocka_unit_test(test_arraylist_add),
	};
	return cmocka_run_group_tests_name("arraylist tests", tests,
		group_setup, group_teardown);
}
