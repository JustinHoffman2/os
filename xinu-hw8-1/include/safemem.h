

/**
 * @file safemem.h
 * Definitions for memory protection and paging functions.
 *
 */
/* Embedded Xinu, Copyright (C) 2023.  All rights reserved. */

#ifndef _SAFEMEM_H_
#define _SAFEMEM_H_

#include <stddef.h>

#define PAGE_SIZE 4096

/** Round a memory address to the upper page boundary */
#define roundpage(x) (((PAGE_SIZE-1) + (ulong)(x)) & ~(PAGE_SIZE-1))
/** Truncate a memory address to the lower page boundary */
#define truncpage(x) ((ulong)(x) & ~(PAGE_SIZE-1))

/**
 * Linked list struct for keeping track of free physical pages
 */
struct pgmemblk
{
    struct pgmemblk *next;
};

extern struct pgmemblk *pgfreelist;     /*      Linked list of physical free pages    */
extern uint pgtbl_nents;        /*      Number of pages in the entire system  */

typedef ulong *pgtbl;
typedef ulong *page;
typedef ulong pte;

/* SATP Register hold the Address Space Identifier (ASID) and the Physical Page Number (PPN)
*  The ASID allows the TLB to detect if the address space has been switched.  In Embedded Xinu, the ASID is the process ID
*  The PPN is the physical address of the root level page table divided by 4096 (PAGE_SIZE)
* +------+--------------------------+----------------------+
* | MODE | Address Space Identifier | Physical Page Number |
* +------+--------------------------+----------------------+
* |    4 |                       16 |                   44 |
* +------+--------------------------+----------------------+
*/
#define SATP_SV39_ON (0x8L << 60)       // This sets the SATP registers's mode to 1000(8 in decimal). This enables Page-based 39-bit virtual addressing
#define SATP_SV39_OFF (0x0L <<60)       //This disables SATP by setting mode to 0000(0 in decimal)
#define MAKE_SATP(asid, pagetable) (SATP_SV39_ON | ((ulong)asid << 44) | (((ulong)pagetable) >> 12))


/* Structure of a page table entry
* +-------+--------+--------+--------+------+-------+--------+--------+--------+------+-------+------+------+
* | 63-54 | 53-28  | 27-19  | 18-10  | 9-8  | 7     | 6      | 5      | 4      | 3    | 2     | 1    | 0    |
* +-------+--------+--------+--------+------+-------+--------+--------+--------+------+-------+------+------+
* | resrv | PPN[2] | PPN[1] | PPN[0] | rsrv | Dirty | Access | Global | U-Mode | Exec | Write | Read | Valid|
* +-------+--------+--------+--------+------+-------+--------+--------+--------+------+-------+------+------+
*/
#define PTE_SIZE 8              // RISC-V Page Table Entries are 8 bytes
#define PTE_MAX  (PAGE_SIZE / PTE_SIZE)
#define PTE_V (1)               // Valid bit indicates if there's a page present for the PTE
#define PTE_R (1 << 1)          // Readable bit indicates that data can be read from the page
#define PTE_W (1 << 2)          // Writeable bit indicates that data can be written to the page
#define PTE_X (1 << 3)          // Executable bit indicates that the page can be executed
#define PTE_U (1 << 4)          // User-mode bit indicates if user-mode code can access the virtual page
#define PTE_G (1 << 5)          // Global bit indicates global mappings. Global mappings are those that exist in all address spaces
#define PTE_A (1 << 6)          // Access bit indicates the virtual page has been read, written, or fetched from since the last time the A bit was cleared.
#define PTE_D (1 << 7)          // Dirty bit indicates the virtual page has been writen to since the last time the dirty bit was cleared.

#define PTE2PA(pte)  ((pte >> 10) * PAGE_SIZE)  // Remove the first 10 bits (any attributes).  Then multiply it by 4096 (page size)
#define PA2PTE(pa)   (((ulong)pa / PAGE_SIZE) << 10)    // Opposite of PTE2PA. Divide by the page size and then make room for flags

#define VAOFFSET 12             //Every virtual address is offset by 12 bits
#define PXMASK 0x1FF            //the virtual addr is masked into 3 sections of 9 bits. these correlate to lvl2,1,0
#define PXSHIFT(level)  (VAOFFSET+(9*(level)))
#define PX(level, addr) ((((ulong) (addr)) >> PXSHIFT(level)) & PXMASK)

/* Prototypes for dealing with physical pages */
void pgInit(void);
void *pgalloc(void);
int pgfree(void *);
int pgfreerange(void *start, void *end);

/* Prototypes mapping virtual addresses to physical addresses */
int mapAddressRange(pgtbl pt, ulong vaddr, ulong paddr, ulong length,
                    int attr);
int mapPage(pgtbl pt, ulong vaddr, ulong paddr, int attr);

/* Prototypes for dealing with physical pages */
pgtbl vm_userinit(int pid, page stack);
pgtbl vm_kerninit(void);

// Flush the TLB by executing an sfence.vma
static inline void sfence_vma(void)
{
    asm volatile ("sfence.vma zero, zero");     // This will flush the entire TLB
}

#endif                          /* _SAFEMEM_H_ */
