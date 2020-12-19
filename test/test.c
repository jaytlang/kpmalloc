#include <stdio.h>
#include <stdlib.h>
#include <kpmalloc.h>

#include "dat.h"

struct var *varlist;

int
main()
{
    varlist = (struct var *)malloc(sizeof(struct var));
    printf("This is a test!\n");
    return 0;
}