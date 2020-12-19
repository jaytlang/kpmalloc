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
    void *allocptr;
    char *name;
    struct var *next;
};

/* As with any other linked list, this base
 * will be the head for the varlist. Since
 * performance isn't a huge issue in testland,
 * I'll take the runtime penalty. All iterations
 * start from this, and it should be initialized
 * to null at startup.
 */
extern struct var *varlist;

#endif /* INC_KPTEST_DAT_H */