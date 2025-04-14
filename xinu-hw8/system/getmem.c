/**
 * @file getmem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * @ingroup memory_mgmt
 *
 * Allocate heap memory.
 *
 *
 * @param nbytes
 *      Number of bytes requested.
 *
 * @return
 *      ::SYSERR if @p nbytes was 0 or there is no memory to satisfy the
 *      request; otherwise returns a pointer to the allocated memory region.
 *      The returned pointer is guaranteed to be 8-byte aligned.  Free the block
 *      with memfree() when done with it.
 */
void *getmem(uint nbytes)
{
    register memblk *prev, *curr, *leftover;

    if (0 == nbytes)
    {
        return (void *)SYSERR;
    }

    /* round to multiple of memblock size   */
    nbytes = (uint)roundmb(nbytes);
    struct memhead *head = (memhead *)proctab[currpid].heaptop;

    /* TODO:
     *      - Traverse through the freelist
     *        to find a block that's suitable 
     *        (Use First Fit with remainder splitting)
     *      - if there is no suitable block, call user_incheap
     *        with the request to add more pages to our process heap
     *      - return memory address if successful
     */
    prev = NULL;
    curr = head->head;
	
    while(curr != NULL) {
    	
        if (curr->length == nbytes + sizeof(memblk)) {
            if (prev == NULL)
                head->head = curr->next;
	    else
	    	prev->next = curr->next;
            return (void *)((ulong)curr + sizeof(memblk));
        }
        
        if (curr->length > nbytes + sizeof(memblk)) {
            leftover = (ulong)curr + nbytes;
            leftover->length = curr->length - nbytes;
            leftover->next = curr->next;
            if (prev == NULL)
                head->head = leftover;
            else
                prev->next = leftover;
            curr->length = nbytes + sizeof(memblk);
            return (void *)((ulong)curr + sizeof(memblk));
        }

        prev = curr;
        curr = curr->next;
    }
    
    return user_incheap(nbytes);
    //return (void *)SYSERR;
}
