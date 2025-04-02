/**
 * @file testcases.c
 * @provides testcases
 *
 *
 * Modified by:	
 *
 * TA-BOT:MAILTO 
 *
 */
/* Embedded XINU, Copyright (C) 2023.  All rights reserved. */

#include <xinu.h>

/* This fake page table will allow you to test your printPageTable function
 * without having paging completely working.
 */
pgtbl createFakeTable(void){
	pgtbl root = pgalloc();
	pgtbl lvl1 = pgalloc();
	pgtbl lvl0 = pgalloc();

	volatile ulong *pte = &(root[5]);
	*pte = PA2PTE(lvl1) | PTE_V;

	ulong *lvl1pte = &(lvl1[145]);
	*lvl1pte = PA2PTE(lvl0) | PTE_V;

	ulong *lvl0pte = &(lvl0[343]);
	*lvl0pte = PA2PTE(0x1000) | PTE_W | PTE_R | PTE_V;

	ulong *lvl0pte1 = &(lvl0[120]);
	*lvl0pte1 = PA2PTE(0x4000) | PTE_X | PTE_R | PTE_V;

	ulong *lvl0pte2 = &(lvl0[45]);
	*lvl0pte2 = PA2PTE(0x8000) | PTE_X | PTE_R | PTE_V;

	return root;
}

void printPageTable(pgtbl pagetable)
{
	/*
	* TODO: Write a function that prints out the page table.
	* Your function should print out all *valid* page table entries in
	* the page table.  If any of the entries are a link (if the
	* Read/Write/Execute bits aren't set), recursively print that page
	* table.  If it is a leaf, print the page table entry and the
	* physical address is maps to. 
	*/

	int i;
	kprintf("PageTable @ PA = 0x%x:\r\n", pagetable);
	for (i = 0; i < 512; i++) {
		if (pagetable[i] & PTE_V) {
			kprintf("\tEntry %d: ", i);
			if (pagetable[i] & (PTE_R | PTE_W | PTE_X)) {
				kprintf("Leaf @ PA = 0x%x\r\n", PTE2PA(pagetable[i]));
			} else {
				kprintf("Link @ PA = 0x%x\r\n", PTE2PA(pagetable[i]));
				printPageTable((pgtbl)PTE2PA(pagetable[i]));
			}
		}	
	}
}

int test_usernone(void) {
	kprintf("Test of user_none\r\n");
	user_none();
	return 1;
}

int test_usermem(void) {
	kprintf("user attempting to overwrite the _kernsp...\r\n");
	_kernsp = (ulong *)0xABBBA;
	return 1;
}

int test_userkern(void) {
	pcb *main;
	main = &proctab[0];
	kprintf("user attempts to read the Main Stack Pointer...\r\n");
	kprintf("Main SP: %lX\r\n", main->ctx[CTX_SP]);
	kprintf("user attempting to write to the Main Stack Pointer...\r\n");
	main->ctx[CTX_SP] = main->ctx[CTX_SP] << 22;
	kprintf("Main SP after trying to write: %lX\r\n", main->ctx[CTX_SP]);
	return 1;
}

int test_usernull(void) {
	int *ptr = NULL;
	kprintf("user attempting to dereference a null pointer...\r\n");
	*ptr = 21;
	return 1;
}

/**
 * testcases - called after initialization completes to test things.
 */
void printpcb(int pid)
{
    pcb *ppcb = NULL;

    /* Using the process ID, access it in the PCB table. */
    ppcb = &proctab[pid];

    /* Printing PCB */
    kprintf("Process name		  : %s \r\n", ppcb->name);

    switch (ppcb->state)
    {
    case PRFREE:
        kprintf("State of the process	  : FREE \r\n");
        break;
    case PRCURR:
        kprintf("State of the process 	  : CURRENT \r\n");
        break;
    case PRSUSP:
        kprintf("State of the process	  : SUSPENDED \r\n");
        break;
    case PRREADY:
        kprintf("State of the process	  : READY \r\n");
        break;
    default:
        kprintf("ERROR: Process state not correctly set!\r\n");
        break;
    }

    /* Print PCB contents and registers */
    kprintf("Base of run time stack    : 0x%08X \r\n", ppcb->stkbase);
    kprintf("Stack length of process   : %8u \r\n", ppcb->stklen);
    kprintf("Stack pointer	       : %lx \r\n", ppcb->ctx[CTX_SP]);
}

void testcases(void)
{
	uchar c;

	kprintf("===TEST BEGIN===\r\n");

	// TODO: Test your operating system!
	
	kprintf("0) user process & page table\r\n");
	kprintf("1) user process memory access\r\n");
	kprintf("2) user process read kernel, not write\r\n");
	kprintf("3) test NULL Pointer Exception\r\n");
	kprintf("4) Test page table print\r\n");
	kprintf("\r\n");
	c = kgetc();
	switch (c)
	{
		case '0':
			// TODO: Write a testcase that creates a user process
			// and prints out it's page table
			
			pid_typ newPid = create((void *)test_usernone, INITSTK, 100, "test_usernone", 0);
			ready(newPid, RESCHED_YES);
			printPageTable(proctab[newPid].pagetable);
			break;
		case '1':
			// TODO: Write a testcase that demonstrates a user
			// process cannot access certain areas of memory
			
			ready(create((void *)test_usermem, INITSTK, 100, "test_usermem", 0), RESCHED_YES);
			break;
		case '2':
			// TODO: Write a testcase that demonstrates a user
			// process can read kernel variables but cannot write
			// to them
			
			ready(create((void *)test_userkern, INITSTK, 100, "test_userkern", 0), RESCHED_YES);
			break;
		case '3':
			// TODO: Extra credit! Add handling in xtrap to detect
			// and print out a Null Pointer Exception.  Write a
			// testcase that demonstrates your OS can detect a
			// Null Pointer Exception.
			
			ready(create((void *)test_usernull, INITSTK, 100, "test_userkern", 0), RESCHED_YES);
			break;
		case '4':
			pgtbl table;
		      	table = createFakeTable();
			printPageTable(table);
			break;
		default:
			break;
	}

	kprintf("\r\n===TEST END===\r\n");
	return;
}
