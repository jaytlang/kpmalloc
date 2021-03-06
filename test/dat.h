#ifndef INC_KPTEST_DAT_H
#define INC_KPTEST_DAT_H

/* Testing infrastructure / data
 * structures for kpmalloc. For now,
 * this constitutes a REPL that allows
 * us to push random cases up it and see
 * if it breaks, or if valgrind complains,
 * etc.
 */

/* We'll need a structure to describe
 * a variable name for a memory allocation.
 * This is (surprise) another linked list,
 * but these vars will utilize the host heap
 * so we can avoid any bugs in kpmalloc while
 * trying to make these.
 */
struct var{
    char *allocptr;
    char *name;
    unsigned int size;
    struct var *next;
};

/* As with any other linked list, this base
 * will be the head for the varlist. Since
 * performance isn't a huge issue in testland,
 * I'll take the runtime penalty. Anywho all iterations
 * start from this guy, which should be initialized to
 * null at startup.
 */
extern struct var *varlist;

/* The parser expects fixed string constants from the
 * user, with the following syntax:
 * -> malloc varname size
 * -> free varname
 * -> status
 * The command-y bits of this are written out here.
 */
#define MALLOC_CMD "malloc"
#define FREE_CMD "free"
#define STATUS_CMD "status"
#define EXIT_CMD "exit"

/* The delimiter for the parser, by default a space. */
#define DELIMITER " "

#endif /* INC_KPTEST_DAT_H */