#include <stdarg.h>
#include <stdio.h>

#include <kpmalloc.h>

#include "dat.h"
#include "fns.h"

void
dbgprintf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    return;
}