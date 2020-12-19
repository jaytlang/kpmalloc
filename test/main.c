#include <stdio.h>
#include <stdlib.h>
#include <kpmalloc.h>

#include "dat.h"
#include "fns.h"

int
main()
{
    printf("Welcome to the test suite!\n");
    for(;;) parseline("> ");
}