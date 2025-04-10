/**
 * @file incmem.c
 * @provides incmem
 */
/* Embedded XINU, Copyright (C) 2023.  All rights reserved. */

#include <xinu.h>

extern void *end;

/**
 * Increase the heap of a user process, returning bottom virtual address
 * of the new memory region.
 * @param size amount of memory to allocate, in bytes
 * @return bottom virtual address of the new memory region
 */
void *incheap(ulong size)
{
    pcb *ppcb;                  /* pointer to proc control blk  */
    ppcb = &proctab[currpid];
    ulong top = 0;
    ulong addr = 0;
    ulong end = 0;
    page pg = NULL;

    // Round size to the next multiple of PAGE_SIZE
    size = roundpage(size);
    struct memhead *mh = (struct memhead *)(ppcb->heaptop);
    top = addr = mh->bound;
    end = top + size;

    // Need a stack check
    if (end > (ulong)PROCSTACKVADDR)
    {
        return (void *)SYSERR;
    }

    // Loop over the entire range
    for (; addr < end; addr += PAGE_SIZE)
    {
        if ((pg = pgalloc()) == (ulong *)SYSERR)
        {
            return ((void *)SYSERR);
        }

        if ((mapAddressRange
             (ppcb->pagetable, addr, (ulong)pg, PAGE_SIZE,
              PTE_R | PTE_W | PTE_U | PTE_A | PTE_D)) == SYSERR)
        {
            return ((void *)SYSERR);
        }
    }
    mh->bound = (ulong *)end;

    return ((ulong *)top);
}
