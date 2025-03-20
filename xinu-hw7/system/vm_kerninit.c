#include <xinu.h>

/**
 * Initialize kernel mappings to include statically configured kernel
 * memory.  Includes kernel text, data, stack, memory region table, and 
 * page table table.
 */
pgtbl vm_kerninit(void)
{
    register pcb *ppcb;
    // Get a page to start a page table via pgalloc
    // This will be the root (level 2) of the hierarchical table
    pgtbl pagetable = pgalloc();

    // Map the UART
    mapAddressRange(pagetable, UART_BASE, UART_BASE, PAGE_SIZE,
                    PTE_R | PTE_W | PTE_A | PTE_D);

    // Map the PLIC
    mapAddressRange(pagetable, PLIC_BASE, PLIC_BASE, 0x4000000,
                    PTE_R | PTE_W | PTE_A | PTE_D);

    // Map the timer regs
    mapAddressRange(pagetable, TIMER_BASE, TIMER_BASE, PAGE_SIZE,
                    PTE_R | PTE_W | PTE_A | PTE_D);

    // Map the kernel code
    mapAddressRange(pagetable, (ulong)&_start, (ulong)&_start,
                    ((ulong)&_ctxsws - (ulong)&_start),
                    PTE_R | PTE_X | PTE_A | PTE_D | PTE_G);

    // Map interrupt and context switch
    mapAddressRange(pagetable, (ulong)&_ctxsws, (ulong)&_ctxsws,
                    PAGE_SIZE + PAGE_SIZE,
                    PTE_R | PTE_X | PTE_A | PTE_D | PTE_G);

    // Map rest of kernel code
    mapAddressRange(pagetable, (ulong)&_interrupte, (ulong)&_interrupte,
                    ((ulong)&_datas - (ulong)&_interrupte),
                    PTE_R | PTE_X | PTE_A | PTE_D | PTE_G);

    // Map global kernel structures and stack
    mapAddressRange(pagetable, (ulong)&_datas, (ulong)&_datas,
                    ((ulong)memheap - (ulong)&_datas),
                    PTE_R | PTE_W | PTE_A | PTE_D);

    // Map entirety of RAM
    mapAddressRange(pagetable, (ulong)memheap, (ulong)memheap,
                    ((ulong)platform.maxaddr - (ulong)memheap),
                    PTE_R | PTE_W | PTE_A | PTE_D);

    ppcb = &proctab[currpid];
    ppcb->pagetable = pagetable;

    // Switch to the kernel page table now
    set_satp(MAKE_SATP(0, pagetable));
    _kernsp = (ulong *)(memheap - PAGE_SIZE - PAGE_SIZE);
    return pagetable;
}
