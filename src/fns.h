#ifndef INC_FNS_H
#define INC_FNS_H

/* Function definitions.
 * Also serves as a place
 * where architecture-specific
 * definitions can be toyed with
 * down the road, as necessary to port
 * to the RISC-V bare metal environment. This'll
 * probably have to happen with kpinit, as well as
 * more complex printing stuff necessary over RISC-
 * attached UART and, you know, the OS...
 */

/* Forward declarations */
struct block;

/* kpinit: Initialize the allocator + the memory it has control
 *      over. On Linux, this will manifest as a one time call
 *      to mmap or sbrk or similar to get us the memory we need,
 *      probably performed by malloc upon a first run. kpinit
 *      initializes a degenerate free list using the sentinel,
 *      and returns an indication of its success or failure.
 * 
 * return: 0 on success, -1 on failure.
 */
int kpinit(void);

/* kpget: equivalent to a call to sbrk or mmap once more, kpget
 *      allows a user application to expand its heap by a given
 *      number of bytes. This is, once again, architecture
 *      dependent - but enables thread local storage via several
 *      different arenas local arenas. Since the end-of-the-day simple
 *      operating system won't share memory at all besides strictly
 *      controlled bounded buffers, this suits our needs.
 * 
 *      If successful, memory will be injected into the freelist, which
 *      we know about globally so no need to pass it. the new value of
 *      the free pointer will be returned to us afterwards, so we can
 *      continue using the best fit policy.
 * 
 * size: how much memory we want from the OS/HAL. Must be >=0 and excludes
 *      the header block.
 * 
 * return: the new memory if successful (i.e. new freep), NULL if failed.
 */
struct block *kpget(unsigned int size);

/* dprintf: print a diagnostic message to stderr. Only should
 *      be run if the DEBUG flag is passed at compile time
 *      (i.e. make debug).
 * 
 * fmt: format string, coinciding with the format typically passed
 *      via printf(1). add additional variadic arguments as 
 *      necessary/permitted.
 */
void dbgprintf(const char *fmt, ...);

#ifdef DEBUG
#define DPRINT 1
#else
#define DPRINT 0
#endif

/* LOG: invoke dprintf iff the DPRINT flag (i.e. DEBUG flag) is
 * defined. Otherwise, do nothing, consuming no ASM instructions.
 * Should be invoked with double parenthesis around function args,
 * to allow the passing of the format string/variadic args without
 * breaking my ANSI C.
 * 
 * fmt: format string, coinciding with the format typically passed
 *      via printf(1). add additional variadic arguments as 
 *      necessary/permitted.
 */
#define LOG(x) do { if (DPRINT) dbgprintf x; } while (0)

#endif /* INC_FNS_H */