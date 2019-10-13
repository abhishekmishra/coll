#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>

#include "test_arraylist.h"

static int group_setup(void **state)
{
    return 0;
}
static int group_teardown(void **state)
{
    return 0;
}

static void test_bogus(void **state)
{
    assert_string_equal("bogus", "bogus");
}

int arraylist_tests()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_bogus),
    };
    return cmocka_run_group_tests_name("docker container tests", tests,
                                       group_setup, group_teardown);
}
