#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kpmalloc.h>

#include "dat.h"
#include "fns.h"

struct var *varlist = NULL;

struct var *
findvar(char *varname)
{
    struct var *vp;

    if(!varlist){
        fprintf(stderr, "\tError: varlist is not initialized, ignoring\n");
        return NULL;
    }

    vp = varlist;
    for(;;){
        if(strcmp(vp->name, varname) == 0){
            fprintf(stderr, "\tfound variable %s.\n", varname);
            return vp;
        }

        if(!vp->next) break;
        else vp = vp->next;
    }
    fprintf(stderr, "\tNo such variable (%s) found\n", varname);
    return NULL;
}

struct var *
allocvar(char *varname, unsigned int size)
{
    struct var *newvar, *vp;

    if(size < 1){
        fprintf(stderr, "\tError: variable size should be >= 1\n");
        goto bad;
    }

    if(strlen(varname) == 0){
        fprintf(stderr, "\tError: variable name can't be empty\n");
        goto bad;
    }

    if(varlist && findvar(varname)){
        fprintf(stderr, "\tError: this variable already exists\n");
        goto bad;
    }

    newvar = (struct var *)malloc(sizeof(struct var));
    if(!newvar){
        fprintf(stderr, "\tError: faled to alloc newvar on host heap\n");
    }
    newvar->allocptr = NULL;
    newvar->next = NULL;
    newvar->name = varname;
    newvar->size = size;

    /* Insert us into the end of the varlist */
    if(varlist == NULL) varlist = newvar;
    else{
        vp = varlist;
        while(vp->next) vp = vp->next;
        vp->next = newvar;
    }

    /* Finally, get memory from kpmalloc. We will get `size` bytes. */
    newvar->allocptr = (char *)kpmalloc(size);
    if(!newvar->allocptr){
        fprintf(stderr, "\tError: failed to allocate out of kpmalloc\n");
        goto worse;
    }

    return newvar;
worse:
    freevar(newvar);
bad:
    return NULL;
}

void
freevar(struct var *varptr)
{
    struct var *vp;
    
    if(!varptr){
        fprintf(stderr, "\tError: varptr is null, ignoring\n");
        return;
    }

    if(!varlist){
        fprintf(stderr, "\tError: varlist is not initialized, ignoring\n");
        return;
    }

    /* Evict us from the varlist */
    vp = varlist;
    while(vp != varptr) vp = vp->next;
    vp->next = varptr->next;
    if(vp == varlist){
        if(varlist->next) varlist = varlist->next;
        else varlist = NULL;
    }

    /* Then, formally free the structure.
     * varptr was allocated on the host heap,
     * as was varptr->name. newvar->allocptr
     * might be null, but if not was alloc'd on
     * the kpmalloc heap.
     */
    free(varptr->name);
    if(varptr->allocptr) kpfree(varptr->allocptr);
    free(varptr);
    return;
}

int
printvarlist(void)
{
    struct var *vp;

    if(!varlist){
        fprintf(stderr, "\tError: varlist is not initialized, can't print\n");
        return -1;
    }

    vp = varlist;
    for(;;){
        printf("\t%s: allocated block of size %d => %p\n",
            vp->name, vp->size, vp->allocptr);
        if(!vp->next) return 0;
        else vp = vp->next;
    }
}