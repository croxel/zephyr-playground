#include <zephyr/ztest.h>

static void before(void *unused)
{
}

static void after(void *unused)
{
}

ZTEST_SUITE(hello_world, NULL, NULL, before, after, NULL);

ZTEST(hello_world, test_hello_world_harness_in_place)
{
	zassert_ok(0, "Harness is installed");
}
