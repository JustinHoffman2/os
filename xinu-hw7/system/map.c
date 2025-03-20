/**
 * @file map.c
 * @provides mapPage, mapAddressRange
 *
 * Modified by:
 *
 * TA-BOT:MAILTO
 */
/* Embedded Xinu, Copyright (C) 2013, 2025.  All rights reserved. */

#include <xinu.h>

syscall mapPage(pgtbl pagetable, ulong vaddr, ulong paddr, int attr);

/**
 * Maps a given virtual address range to a corresponding physical address range.
 * @param pagetable    base pagetable
 * @param vaddr        start of the virtual address range.
 *                     This should be truncated to the nearest page boundry.
 * @param paddr        start of the physical address range
 * @param length       length of the range to map
 * @param attr         any attributes to set on the page table entry
 * @return             OK if successful, otherwise a SYSERR
 */
syscall mapAddressRange(pgtbl pagetable, ulong vaddr, ulong paddr,
                        ulong length, int attr)
{
    ulong end;

    // DEBUGGING LINE:
    //    kprintf("mapAddressRange(0x%lX, 0x%lX, 0x%lX, 0x%lX, 0x%lX)\r\n",
    //      pagetable, vaddr, paddr, length,  attr);

    if (length == 0)
    {
        return SYSERR;
    }

    // Round the length, vaddr and paddr to the nearest page size
    length = roundpage(length);
    vaddr = (ulong)truncpage(vaddr);
    paddr = (ulong)truncpage(paddr);
    end = vaddr + length;

    // Loop over the entire range
    for (; vaddr < end; vaddr += PAGE_SIZE, paddr += PAGE_SIZE)
    {
        // Map the individual virtual pages into the table.
        if (mapPage(pagetable, vaddr, paddr, attr) == SYSERR)
        {
            return SYSERR;
        }
    }

    return OK;
}

/**
 * Maps a page to a specific virtual address
 * @param pagetable  base pagetable
 * @param vaddr      virtual address at which the page will reside
 * @param paddr      physical frame that will be mapped at the virtual address
 * @param attr       any attributes to set on the page table entry
 * @return           OK if successful, otherwise a SYSERR
 */
syscall mapPage(pgtbl pagetable, ulong vaddr, ulong paddr, int attr)
{
/**
 * Starting at the base pagetable, traverse the hierarchical page table
 *  structure for the virtual address.  Create pages along the way if they
 *  don't exist.
 */

    /**
    * TODO:
    * For each level in the page table, get the page table entry by masking
    * and shifting the bits in the virtualaddr depending on the level.
    * If the valid bit is set, use that pagetable for the next level
    * Otherwise create the page by calling pgalloc().  Make sure to setup the 
    * page table entry accordingly. Call sfence_vma once finished to flush TLB
    * Once you've traversed all three levels, set the attributes (attr) for
    * the leaf page (don't forget to set the valid bit!)
    */

    //  DEBUGGING LINE:
    //  kprintf("mapPage(pt:0x%X, v:0x%X, p:0x%0X, a:0x%03X)\r\n",
    //          pagetable, vaddr, paddr, attr);

    return OK;
}
