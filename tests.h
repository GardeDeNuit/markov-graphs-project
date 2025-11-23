#ifndef TESTS_H
#define TESTS_H

/**
 * @file tests.h
 * @brief Framework API for unit tests registration and execution.
 */

/**
 * @brief Function pointer type for test functions.
 *
 * Test functions must take no parameters and return 0 on success, non-zero on failure.
 */
typedef int (*test_fn)(void);

/**
 * @brief Represents a registered test entry.
 *
 * @param name Human-readable name of the test.
 * @param fn Function pointer to the test function.
 * @param comment Optional short description of the test.
 */
typedef struct {
    const char *name;
    test_fn fn;
    const char *comment;
} test_entry;

/**
 * @brief Register a test in the global test registry.
 *
 * @param name Human-readable name of the test (must be a valid C string).
 * @param fn Pointer to the test function (must conform to test_fn).
 * @param comment Optional short description of the test (may be NULL).
 */
void add_test(const char *name, test_fn fn, const char *comment);

/**
 * @brief Execute all registered tests and print results.
 *
 * @return Number of failed tests (0 indicates all tests passed).
 *
 * Each test is executed in registration order.
 */
int run_all_tests(void);

/**
 * @brief Register built-in project tests into the global registry.
 *
 * Call this function once before run_all_tests() to populate the registry with
 * the default unit tests.
 */
void register_project_tests(void);

#endif // TESTS_H
