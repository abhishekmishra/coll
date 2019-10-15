#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>

#include "test_arraylist.h"
#include "arraylist.h"

static int group_setup(void** state) {
	return 0;
}
static int group_teardown(void** state) {
	return 0;
}

static void test_arraylist_new_create_only(void** state) {
	arraylist* l;
	int err = arraylist_new(&l, NULL);
	assert_int_equal(err, 0);
}

int arraylist_tests() {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_arraylist_new_create_only),
	};
	return cmocka_run_group_tests_name("arraylist tests", tests,
		group_setup, group_teardown);
}
