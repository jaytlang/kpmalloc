#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kpmalloc.h>

int
main()
{
    char *mystr;

    printf("Welcome to the scratchpad!\n");
    mystr = (char *)kpmalloc(100 * sizeof(char));
    strcpy(mystr, "hello, world");
    printf("Copied in: %s\n", mystr);
    kpfree(mystr);
    printf("Successfully freed\n");
    return 0;
}