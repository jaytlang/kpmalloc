#ifndef INC_KPMALLOC_H
#define INC_KPMALLOC_H

/* kpmalloc: allocate a block of uninitialized memory from the kpmalloc
 *      heap, aligned for the CPU to best take advantage of. respects
 *      the malloc API, see malloc(3) for more details
 * 
 * size: number of bytes to allocate in memory. must be greater than
 *      zero in order for malloc to succeed. 
 * return: a void pointer to the newly allocated memory region, or
 *      NULL if an error has occurred. If debugging is enabled, this
 *      is accompanied by a print to stderr.
 */
void *kpmalloc(unsigned int size);

/* kpfree: deallocate a previously allocated block of memory located
 * at the pointer passed. If this pointer has already been deallocated,
 * or is otherwise untouched by malloc, has no effect and may result in
 * undefined behavior. however, all memory passed to free should be
 * assumed freed as defined in free(3).
 * 
 * Internal use notes; you can also use kpfree to pull in new blocks
 * from mapped memory IFF it is properly prefixed with a block header
 * one unit ahead, and the freelist is already initialized through a
 * call to kpinit.
 *
 * ptr: pointer we would like to deallocate. must have been previously
 *      allocated by malloc and NOT YET FREED.
 */
void kpfree(void *ptr);

#endif /* INC_KPMALLOC_H */