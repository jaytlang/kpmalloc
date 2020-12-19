#ifndef INC_KPTEST_FNS_H
#define INC_KPTEST_FNS_H

struct var;

/* The parser. Internally utilizing the system
 * readline, will grab a line from stdin with
 * the desired prompt, and return you a var structure.
 * This var structure will have been inserted into
 * the varlist already via the var helper methods,
 * so no need to worry about that. However, it can
 * still be removed directly via use of freevar.
 * 
 * prompt: typical string to present to the user.
 * returns: a var structure allocated on the heap
 *      corresponding to the user's response. NULL
 *      if the user punches in something bad, it's
 *      the caller's responsibility to figure it out.
 *      it's also the caller's job to free this later.
 */
struct var *parseline(const char *prompt);

/* Allocate a var object with a string and a size.
 * This constitutes checking if it's on the varlist,
 * calling kpmalloc, and throwing it on the varlist
 * for bookkeeping if everything goes according to
 * plan. The new pointer kpmalloc gives us for user
 * memory is set up, completing the structure.
 * 
 * varname: *heap-allocated* reference to the name
 *      we want to give this thing, s.t. we can
 *      free it down the line if we so choose. Note
 *      that all varnames must be unique; this is
 *      checked at runtime of the test suite.
 * size: integer size (>0) of the allocation we want
 *      to make.
 * return: valid var object inserted into the varlist,
 *      s.t. other internal var methods can call this wo/
 *      issue. if size is bad, the variable already exists,
 *      or insertion is for some reason impossible, return
 *      a null pointer.
 */
struct var *allocvar(char *varname, unsigned int size);

/* Free a var object. This object must be resident
 * in the varlist at the time of calling for this
 * to work properly -- I don't see a situation where
 * that isn't the case tbh. Assume this succeeds
 * no matter what, per typical free tradition.
 * 
 * varptr: reference to the variable to destroy/unlink.
 */
void freevar(struct var *varptr);

/* Find a var object within the free list: given a
 * variable name, return the corresponding reference
 * which can then be inspected, passed to freevar, etc.
 *
 * varname: variable name to hunt down for. Note that
 * allocvar imposes an invariant on the varlist structure
 * that you can only have one variable with a given name,
 * which guarantees this to work.
 * return: a valid variable reference on success. NULL on failure.
 */
struct var *findvar(char *varname);

#endif /* INC_KPTEST_FNS_H */