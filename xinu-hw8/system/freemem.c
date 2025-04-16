/**
 * @file freemem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup memory_mgmt
 *
 * Frees a block of heap-allocated memory.
 *
 * @param memptr
 *      Pointer to memory block allocated with getmem().
 *
 * @param nbytes
 *      Length of memory block, in bytes.  (Same value passed to getmem().)
 *
 * @return
 *      ::OK on success; ::SYSERR on failure.  This function can only fail
 *      because of memory corruption or specifying an invalid memory block.
 */
syscall freemem(void *memptr, uint nbytes)
{
    register struct memblock *block, *next, *prev;
    struct memhead *head = NULL;
    ulong prev_top;
    ulong block_top;

    /* make sure block is in heap */
    if ((0 == nbytes)
        || ((ulong)memptr < (ulong)proctab[currpid].heaptop))
    {
        return SYSERR;
    }

    head = (struct memhead *)proctab[currpid].heaptop;
    block = (struct memblock *)memptr;
    nbytes = (uint)roundmb(nbytes);
    
    next = head->head;
    prev = NULL;

    while (next != NULL && next < block) {
        prev = next;
        next = next -> next;	
    }       

    if (prev != NULL)
        prev->next = block;
    block->next = next;
    block->length = nbytes;
	
    if (prev != NULL) {
        prev_top = (ulong)prev + prev->length;
	if ((ulong)block < prev_top)
	    return SYSERR;
        if (prev_top == (ulong)block) {
            prev->length += block->length;
            prev->next = block->next;
            block = prev;
        }
    } else
	head->head = block;

    if (next != NULL) {
        block_top = (ulong)block + block->length;
	if (block_top > (ulong)next)
	    return SYSERR;
        if (block_top == (ulong)next) {
            block->length += next->length;
            block->next = next->next;
        }
    }
	
    /* TODO:
     *      - Find where the memory block should
     *        go back onto the freelist (based on address)
     *      - Find top of previous memblock
     *      - Make sure block is not overlapping on prev or next blocks
     *      - Coalesce with previous block if adjacent
     *      - Coalesce with next block if adjacent
     */

    return OK;
}
