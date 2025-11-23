#ifndef __UTILS_H__
#define __UTILS_H__

#define TRUE 1
#define FALSE 0

#define STACK_EMPTY (-1)

/**
 * @brief Stack cell structure
 */
typedef struct s_stack_cell{
    int value;
    struct s_stack_cell *next;
}t_stack_cell;

/**
 * @brief Stack structure
 */
typedef struct {
    t_stack_cell *top;
}t_stack;

/**
 * @brief Validates if an integer is within a specified range
 * @param value The value to validate
 * @param min Minimum acceptable value (inclusive)
 * @param max Maximum acceptable value (inclusive)
 * @param errMsg Error message to display if validation fails
 * @return 1 if valid, 0 otherwise
 */
int validateIntRange(int value, int min, int max, const char *errMsg);

/**
 * @brief Converts an integer to an Excel-style column ID (1=A, 2=B, ..., 27=AA)
 * @param i The integer to convert (1-based)
 * @return Dynamically allocated string containing the ID (caller must free), or NULL on error
 */
char *getID(int i);

/**
 * @brief Creates an empty stack
 * @return Pointer to the newly created stack
 */
t_stack * createStack(void);

/**
 * @brief Checks if a stack is empty
 * @param stack The stack to check
 * @return 1 if empty, 0 otherwise
 */
int isStackEmpty(t_stack *);

/**
 * @brief Pushes a value onto the stack
 * @param stack The stack to push onto
 * @param value The value to push
 */
void pushStack(t_stack *, int);

/**
 * @brief Pops a value from the stack
 * @param stack The stack to pop from
 * @return The popped value, or STACK_EMPTY if the stack is empty
 */
int popStack(t_stack *);

/**
 * @brief Frees all cells in a stack recursively
 * @param cell The first cell to free
 */
void FreeStackCells(t_stack_cell *);

/**
 * @brief Frees an entire stack and all its cells
 * @param stack The stack to free
 */
void freeStack(t_stack *);

/**
 * @brief Returns the minimum of two integers
 * @param a First integer
 * @param b Second integer
 * @return The smaller of the two values
 */
int minInt(int, int);

/**
 * @brief Returns the maximum of two integers
 * @param a First integer
 * @param b Second integer
 * @return The larger of the two values
 */
int maxInt(int, int);

/**
 * @brief Converts an integer to a string
 * @param value The integer to convert
 * @return Static string buffer containing the converted value
 * @warning The returned buffer is static and will be overwritten on next call
 */
char *intToStr(int);

/**
 * @brief Prints a debug message if debug mode is enabled
 * @param isDebugMode 1 to enable debug output, 0 to disable
 * @param msg The message to print
 */
void debugPrint(int isDebugMode, const char *msg);


#endif