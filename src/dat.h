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

/* How large should functions like kpinit attempt
 * to make the mempool? We will start with the size
 * offered by the RV32 board I'd like to port to, to
 * keep things simple.
 */
#define MEMPOOL_SZ 0x3fff

#endif /* INC_DAT_H */