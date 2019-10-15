/**
 * Template for tests.
 * All testing macros needed for project can be found at [1]
 *
 * [1] http://cpputest.github.io/manual.html#test_macros
 */

// Provides test macros
#include <CppUTest/TestHarness.h>

// All functions written for project IFJ must be
// included in this block:
extern "C" {
}

const char *testString()
{
	return "test";
}

/**
 * Every written test must belong into group.
 * Typically every test module has its own test group.
 * To create group can be used:
 */
TEST_GROUP(TestGroup)
{
};

/**
 * Every test must belong to test group and must
 * have its unique name.
 */
TEST(TestGroup, StringTest)
{
    // compares null terminated const char *
    STRCMP_EQUAL(testString(), "tes");
}

// Don't forget to add test into Makefile!
