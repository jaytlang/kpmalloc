/* Architecture specific kpmalloc
 * implementation, first for Linux.
 */
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <kpmalloc.h>

#include "dat.h"
#include "fns.h"

int
kpinit(void)
{
    /* We need at least 1 core to continue, and
     * the sentinel will be the new free pointer
     * when kpget finishes for the first time
     */
    sentinel = kpget(INITIAL_ALLOC_UNITS);
    if(sentinel == NULL){
        LOG(("bootstrap kpget invocation failed!\n"));
        return -1;
    }else LOG(("bootstrap kpget invocation succeeded\n"));

    /* Problem solved. Link us up with
     * ourselves properly, and the size should
     * get properly set to a degenerate value of 0.
     */
    sentinel->next = sentinel;
    return 0;
}

struct block *
kpget(unsigned int blocks)
{
    void *newmem;
    struct block *newhdr;
    int fd;

    /* Need to allocate a header for these things
     * in conjunction with the actual data requested
     */
    blocks++;

    /* Under Linux/BSD, we mmap. */
    fd = open("/dev/zero", O_RDONLY);
    if(fd < 0){
        LOG(("Failed to open /dev/zero for mmap\n"));
        return NULL;
    }

    newmem = mmap(0, blocks * sizeof(struct block), PROT_READ | PROT_WRITE,
                MAP_PRIVATE, fd, 0);
    
    if(newmem == MAP_FAILED){
        LOG(("Failed to map in %d blocks from /dev/zero\n", blocks));
        return NULL;
    }

    memset(newmem, 0, blocks * sizeof(struct block));
    newhdr = (struct block *)newmem;
    newhdr->next = NULL;
    newhdr->size = blocks - 1;

    /* If the freelist is null, this a bootstrap operation called
     * from kpinit. Precondition does not forbid this... so 
     * given that this is the case, set up the freelist
     * in addition to the sentinel (which we just allocated)
     * 
     * Otherwise, we set this up with kpfree.
     */
    if(!freelist) freelist = newhdr;
    else kpfree(newhdr + 1);

    LOG(("mapped in %d blocks of memory successfully\n", blocks));
    return freelist;
}
