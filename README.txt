==========
 kpmalloc
==========

A simple, first-iteration memory
allocator for embedded systems.
Looks a lot like the K&R first-fit
malloc implementation, but cleaned
up and integrated with RISC-V PMP
primitives to allow for general
use on MMU-less systems.

The objective here is to get a
system which allows the use of PMP
instead of paging. This is obviously
just a proof of concept - the novelty
at hand is less about the actual
malloc used and more about throwing
PMP into it: demonstrating that we
can have a properly isolated OS
offering conventional abstractions
without an MMU.

By staying simple, we can
demonstrate nesting this system
by initializing it within its own
allocated block -- offering a 'cage'
for user memory to operate out of.
PMP can be added later to isolate this
cage from the outside world, and calls
can be added which grant the cage the
ability to request more memory for
itself (a la sbrk(2)).

Building: type 'make'.
