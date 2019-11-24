#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <stdio.h>

#include "test_lualib_arraylist.h"
#include "test_arraylist.h"
#include "arraylist.h"

static arraylist* intptrlist0;

void intptr_to_lua_number(lua_State* L, void* data) {

}

static int group_setup(void** state) {
	int err = arraylist_new(&intptrlist0, (arraylist_free_function*)&free_intptr);
	set_lua_convertor(intptrlist0, &intptr_to_lua_number);
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
}

int lualib_arraylist_tests() {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_arraylist_new_create_only),
	};
	return cmocka_run_group_tests_name("lualib arraylist tests", tests,
		group_setup, group_teardown);
}
