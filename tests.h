#ifndef TESTS_H
#define TESTS_H

#define RESET_ALL          "\x1b[0m"

#define COLOR_BLACK        "\x1b[30m"
#define COLOR_RED          "\x1b[31m"
#define COLOR_GREEN        "\x1b[32m"
#define COLOR_YELLOW       "\x1b[33m"
#define COLOR_BLUE         "\x1b[34m"
#define COLOR_MAGENTA      "\x1b[35m"
#define COLOR_CYAN         "\x1b[36m"
#define COLOR_WHITE        "\x1b[37m"

#define BACKGROUND_BLACK   "\x1b[40m"
#define BACKGROUND_RED     "\x1b[41m"
#define BACKGROUND_GREEN   "\x1b[42m"
#define BACKGROUND_YELLOW  "\x1b[43m"
#define BACKGROUND_BLUE    "\x1b[44m"
#define BACKGROUND_MAGENTA "\x1b[45m"
#define BACKGROUND_CYAN    "\x1b[46m"
#define BACKGROUND_WHITE   "\x1b[47m"

#define STYLE_BOLD         "\x1b[1m"
#define STYLE_ITALIC       "\x1b[3m"
#define STYLE_UNDERLINE    "\x1b[4m"


// Simple test framework API
typedef int (*test_fn)(void);

// Simple internal test registry
typedef struct {
    const char *name;
    test_fn fn;
    const char *comment;
} test_entry;

// Register a test (name, function, comment)
void add_test(const char *name, test_fn fn, const char *comment);

// Run all registered tests and print result per test. Returns number of failed tests.
int run_all_tests(void);

// Helper to register built-in tests
void register_project_tests(void);


#endif // TESTS_H

