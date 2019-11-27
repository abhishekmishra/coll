#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "test_coll_arraylist_map.h"
#include "coll_arraylist_map.h"

static coll_al_map* map;

static int group_setup(void** state) {
	map = make_coll_al_map(&strcmp);
	if (map == NULL) {
		return -1;
	}

	for (int i = 0; i < 5; i++) {
		char* key = (char*)calloc(3, sizeof(char));
		if (key == NULL) {
			return -1;
		}
		sprintf(key, "%d", i);

		int* val = (int*)calloc(1, sizeof(int));
		if (val == NULL) {
			return -1;
		}
		*val = i;

		coll_al_map_put(map, key, val);
	}
	return 0;
}
static int group_teardown(void** state) {
	free_coll_al_map(map);
	return 0;
}

static void test_coll_al_map_create_new(void** state) {
	coll_al_map* test_map;
	test_map = make_coll_al_map(&strcmp);
	assert_non_null(test_map);
	if (test_map != NULL) {
		free_coll_al_map(test_map);
	}
}

void print_k_v(size_t i, char* key, int* val) {
	printf("[%zu] %2s = %2d\n", i, key, *val);
}

static void test_coll_al_map_foreach(void** state) {
	coll_al_map_foreach(map, &print_k_v);
}

int coll_al_map_tests() {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_coll_al_map_create_new),
		cmocka_unit_test(test_coll_al_map_foreach),
	};
	return cmocka_run_group_tests_name("arraylist map tests", tests,
		group_setup, group_teardown);
}
