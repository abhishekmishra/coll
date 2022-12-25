#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <stdio.h>

#include "test_coll_lualib_arraylist.h"
#include "test_coll_arraylist.h"
#include "coll_arraylist.h"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

static arraylist *intptrlist0;
static lua_State *L;
#define TEST_ARRAYLIST "TestArrayList"

static int get_random_ints(lua_State *L)
{
	convert_to_lua_array(intptrlist0, L);
	return 1;
}

// fill an arraylist of ints with some random integers
static void fill_random_ints(arraylist *l, int num_ints)
{
	for (int i = 0; i < num_ints; i++)
	{
		int *x = (int *)calloc(1, sizeof(int));
		if (x != NULL)
		{
			*x = rand() % 1000;
			arraylist_add(l, x);
		}
	}
}

// this function converts an int pointer
// to a lua numeric value
void intptr_to_lua_number(lua_State *L, int index, void *data)
{
	int* x = (int*)data;
	//printf("setting value %d at %d\n", *x, index+1);
	lua_pushinteger(L, *x);
}

static int group_setup(void **state)
{
	L = luaL_newstate();
	luaL_openlibs(L);

    //Expose the c_swap function to the lua environment
    lua_pushcfunction(L, get_random_ints);
    lua_setglobal(L, "get_random_ints");

	int err = arraylist_new(&intptrlist0, (arraylist_free_function *)&free_intptr);
	set_lua_convertor(intptrlist0, &intptr_to_lua_number);
	fill_random_ints(intptrlist0, 6);
	return err;
}

static int group_teardown(void **state)
{
	arraylist_free(intptrlist0);
	lua_close(L);
	return 0;
}

static char* lua_code = "\n"
"print('Generated arraylist of 6 random integers and converted to LUA array...')\n"
"x = get_random_ints()\n"
"for k, v in ipairs(x) do\n"
"  print(k .. '=' .. v)\n"
"end"
;

static void test_arraylist_list_items(void **state)
{
	// for (int i = 0; i < arraylist_length(intptrlist0); i++)
	// {
	// 	printf("%d\n", *(int *)arraylist_get(intptrlist0, i));
	// }
	luaL_dostring(L, lua_code);
}

int lualib_arraylist_tests()
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_arraylist_list_items),
	};
	return cmocka_run_group_tests_name("lualib arraylist tests", tests,
									   group_setup, group_teardown);
}
