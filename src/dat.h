/* kpmalloc data structures. */
#ifndef INC_DAT_H
#define INC_DAT_H

/* Block data structure -- comprising
 * a circular linked list of free elements.
 * Newly allocated blocks are removed from
 * the free list, their headers left intact
 * but the pointer now rendered useless until
 * next use. Freed blocks have an accurate size
 * field and are part of the linked list -- with
 * the invariant that blocks are laid out
 * contiguously in memory from left to right
 * (besides the reset)
 */
struct block{
    unsigned int size;
    struct block *next;
}__attribute__((aligned));

/* Global sentinel/base block reference. Allocated
 * during initialization, set with arbitrary
 * size 0, and used by malloc furthermore.
 */
extern struct block *sentinel;
extern struct block *freelist;

/* Initial number of units/blocks to have kpinit grab
 * during its setup invocation. Minimum 1, but could
 * totally be bigger, because a size of 1 will likely result
 * in a second call to kpget during the first few allocs. I'm
 * gonna trivially set this to 1 for now though to make sure
 * everything is going as intended.
 */
#define INITIAL_ALLOC_UNITS 1

#endif /* INC_DAT_H */