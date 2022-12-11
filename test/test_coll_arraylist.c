#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef APR_ENABLED

#include <apr.h>
#include <apr_pools.h>

#endif //APR_ENABLED

#include "test_coll_arraylist.h"
#include "coll_arraylist.h"

static arraylist* intptrlist0;

void free_intptr(int* p) {
	free(p);
}

void print_intptr(int* p) {
	printf("%d", *p);
}

static int group_setup(void** state) {
	int err = arraylist_new(&intptrlist0, (arraylist_free_function*)&free_intptr);
	return err;
}
static int group_teardown(void** state) {
	arraylist_free(intptrlist0);
	return 0;
}

static void test_arraylist_new_create_only(void** state) {
	arraylist* test_l;
	int err = arraylist_new(&test_l, NULL);
	assert_int_equal(err, 0);
	arraylist_free(test_l);
}

static void test_arraylist_new_create_and_free(void** state) {
	arraylist* test_l;
	int err = arraylist_new(&test_l, free);
	
	int* x = (int*)calloc(1, sizeof(int));
	assert_non_null(x);
	*x = 10;

	arraylist_add(test_l, x);

	assert_int_equal(err, 0);
	arraylist_free(test_l);
}

static void test_arraylist_new_create_and_free_with_item_free(void** state) {
	arraylist* test_l;
	int err = arraylist_new(&test_l, (arraylist_free_function*)free_intptr);

	int* x = (int*)calloc(1, sizeof(int));
	assert_non_null(x);
	*x = 10;

	arraylist_add(test_l, x);

	assert_int_equal(err, 0);
	arraylist_free(test_l);
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
	arraylist_print(intptrlist0, (arraylist_free_function*)print_intptr);
	for (int i = 0; i < 5; i++) {
		assert_int_equal(*((int*)(arraylist_get(intptrlist0, i))), 4ULL - i);
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
	arraylist_print(intptrlist0, (arraylist_free_function*)print_intptr);
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

static void test_arraylist_add_simple(void** state) {
	for (int i = 0; i < 5; i++) {
		int* x = (int*)calloc(1, sizeof(int));
		assert_non_null(x);
		if (x != NULL) {
			*x = i;
			arraylist_add(intptrlist0, x);
		}
	}
	arraylist_print(intptrlist0, (arraylist_free_function*)print_intptr);
	for (int i = 0; i < 5; i++) {
		assert_int_equal(*((int*)(arraylist_get(intptrlist0, i))), i);
	}
	arraylist_clear(intptrlist0);
}

static void test_arraylist_delete_at_beginning(void** state) {
	for (int i = 0; i < 5; i++) {
		int* x = (int*)calloc(1, sizeof(int));
		assert_non_null(x);
		if (x != NULL) {
			*x = i;
			arraylist_add(intptrlist0, x);
		}
	}
	arraylist_print(intptrlist0, (arraylist_free_function*)print_intptr);
	for (int i = 0; i < 5; i++) {
		assert_int_equal(*((int*)(arraylist_get(intptrlist0, i))), i);
	}
	for (int i = 0; i < 3; i++) {
		assert_int_equal(arraylist_delete(intptrlist0, 0), 0);
	}
	for (int i = 0; i < 2; i++) {
		assert_int_equal(*((int*)(arraylist_get(intptrlist0, i))), i + 3ULL);
	}
	arraylist_print(intptrlist0, (arraylist_free_function*)print_intptr);
	arraylist_clear(intptrlist0);
}

#ifdef APR_ENABLED

static void test_arraylist_apr_all(void** state) {
	apr_initialize();

	apr_pool_t* pool;
	apr_status_t res;

	res = apr_pool_create(&pool, NULL);
	if(res != APR_SUCCESS) {
		fail_msg("Unable to allocate apr pool\n");
	}

	arraylist* pool_al;
	res = arraylist_apr_new(&pool_al, pool, (arraylist_free_function*)&free_intptr);
	if(res != 0) {
		fail_msg("Unable to create arraylist");
	}

	for (int i = 0; i < 655; i++) {
		int* x = (int*)calloc(1, sizeof(int));
		assert_non_null(x);
		if (x != NULL) {
			*x = i;
			arraylist_add(pool_al, x);
		}
	}
	arraylist_print(pool_al, (arraylist_free_function*)print_intptr);
	for (int i = 0; i < 5; i++) {
		assert_int_equal(*((int*)(arraylist_get(pool_al, i))), i);
	}
	arraylist_clear(pool_al);

	arraylist_apr_free(pool_al);

	apr_pool_destroy(pool);
	apr_terminate();
}

#endif //APR_ENABLED

int arraylist_tests() {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_arraylist_new_create_only),
		cmocka_unit_test(test_arraylist_new_create_and_free),
		cmocka_unit_test(test_arraylist_new_create_and_free_with_item_free),
		cmocka_unit_test(test_arraylist_insert_at_beginning),
		cmocka_unit_test(test_arraylist_insert_at_end),
		cmocka_unit_test(test_arraylist_insert_beyond_capacity),
		cmocka_unit_test(test_arraylist_insert_large),
		cmocka_unit_test(test_arraylist_add_simple),
		cmocka_unit_test(test_arraylist_delete_at_beginning),
#ifdef APR_ENABLED
		cmocka_unit_test(test_arraylist_apr_all),	
#endif //APR_ENABLED

	};
	return cmocka_run_group_tests_name("arraylist tests", tests,
		group_setup, group_teardown);
}
