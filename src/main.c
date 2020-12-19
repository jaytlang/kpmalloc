#include <stdlib.h>

#include <kpmalloc.h>

#include "dat.h"
#include "fns.h"

struct block *sentinel = NULL;
struct block *freelist = NULL;

void *
kpmalloc(unsigned int size)
{
    unsigned int blocks;
    struct block *previous, *current;

    if(size < 1){
        LOG(("Bad argument: size should be >0\n"));
        return NULL;
    }

    blocks = ((size + sizeof(struct block) - 1) / sizeof(struct block)) + 1;
    if(freelist == NULL){
        if(kpinit() < 0){
            LOG(("Failed to initialize kpmalloc!\n"));
            return NULL;
        }else LOG(("Successfully initialized kpmalloc\n"));
    }


    previous = freelist;
    current = freelist->next;
    for(;; previous = current, current = current->next){
        LOG(("Current size: %d vs. %d needed\n", current->size, blocks));
        if(current->size == blocks){
            /* Good to go. Remove the block from
             * the free list without modification,
             * though i'm setting this thing to null
             * to avoid some leakage of info.
             */
            previous->next = current->next;
            current->next = NULL;

            /* Next invocation starts at old curr->next,
             * or previous->next now, jumping over the alloc.
             */
            freelist = previous;
            return (void *)(current + 1);
        }
        else if(current->size > blocks){
            current->size -= blocks;
            current += current->size;

            current->next = NULL;
            current->size = blocks;

            /* Next invocation starts at previous->next, which
             * is decreased-size current before we jumped by
             * `blocks` forward to the tail of the block.
             */
            freelist = previous;
            return (void *)(current + 1);
        }
        else if(current == freelist){
            /* Freelist will get updated to point right
             * at the new block. Incidentally, this will
             * also be ensured next go-around by
             * freelist = previous, bc we're guaranteed
             * to hit next time. Pretty damn cool.
             */
            current = kpget(blocks);
            if(current == NULL){
                LOG(("Supplemental memory kpget failed. OOM?\n"));
                return NULL;
            }
            LOG(("Finished supplementary kpget request\n"));
        }
    }
}

void
kpfree(void *ptr)
{
    struct block *thisblock, *current, *previous;

    if(!ptr){
        LOG(("Null pointer passed to free, ignoring\n"));
        return;
    }

    thisblock = (struct block *)ptr - 1;
    LOG(("Freeing block of size %d...\n", thisblock->size));

    /* Find the right slot for us in the freelist from an
     * ordering perspective...
     */
    for(current = freelist;
        !(current < thisblock && current->next > thisblock);
        current = current->next){

        /* Special case: we could be the
         * rightmost/leftmost in the list
         */
        if(current >= current->next){
            if(current < thisblock || current->next > thisblock)
                break;
        }
    }

    /* We're all good to go for insertion now...current is
     * equal to where we want to insert ourselves post-spin.
     * Time to link ourselves, and coalesce if we can.
     */
    previous = current;
    current = current->next;

    /* Right adjacency: consume current */
    if(thisblock + thisblock->size == current){
        thisblock->size += current->size;
        thisblock->next = current->next;
    }else{
        /* Size is already set from malloc */
        thisblock->next = current;
    }

    /* Left-adjacency? Previous eats us*/
    if(previous + previous->size == thisblock){
        /* thisblock->size set from malloc */
        previous->size += thisblock->size;
        /* thisblock->next set by last step */
        previous->next = thisblock->next;
    }else{
        previous->next = thisblock;
    }

    /* All done */
    freelist = previous;
    LOG(("Free successful\n"));
    return;
}