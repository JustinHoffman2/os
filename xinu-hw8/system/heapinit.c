/**
 * @file heapinit.c
 * @provides heapinit
 */
/* Embedded XINU, Copyright (C) 2025.  All rights reserved. */

#include <xinu.h>

page heapinit(pcb *ppcb)
{
    page procheap = pgalloc();
    if (SYSERR == procheap)
        return (page) SYSERR;

    struct memhead *mh = (struct memhead *)procheap;
    struct memblock *firstBlock = (struct memblock *)(mh + 1);
    firstBlock->next = NULL;
    firstBlock->length = PAGE_SIZE - sizeof(struct memhead);

    mh->head =
        (struct memblock *)(PROCHEAPVADDR + sizeof(struct memhead));
    mh->length = PAGE_SIZE - sizeof(struct memhead);
    mh->bound = (ulong)(mh->head) + mh->length;
    mh->base = (ulong)mh->head;

    ppcb->heaptop = (ulong *)PROCHEAPVADDR;

    return procheap;
}
