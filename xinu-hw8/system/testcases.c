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

void printfreelist(void){
	register struct memblock *block;
	struct memhead *head = NULL;

	head = (struct memhead *)proctab[currpid].heaptop;
	block = head->head;
	kprintf("Free List:\r\n");
	while(block != NULL){
		kprintf("0x%lx:\r\n",block);
		block = block->next;
	} 
}

void mallocarray(void){

	kprintf("Free list before \r\n");
	printfreelist();
	int *array;
	int n= 10;
	array = (int*) malloc(n*sizeof(int));
	for(int i = 0; i<n; i++){
		array[i] = i + 1;
		kprintf("%d\t%lx\r\n", array[i], &array[i]);

	}
	kprintf("Free list after \r\n");
	printfreelist();
}
/**
 * testcases - called after initialization completes to test things.
 */
void printpcb(int pid) {
    pcb *ppcb = NULL;
    /* Using the process ID, access it in the PCB table. */
    ppcb = &proctab[pid];
    /* Printing PCB */
    kprintf("Process name		  : %s \r\n", ppcb->name);
    switch (ppcb->state) {
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
	
	kprintf("0) test of malloc\r\n");
	kprintf("1) print the free list\r\n");
	kprintf("\r\n");
	c = kgetc();
	switch (c)
	{
		case '0':
			ready(create((void *)mallocarray, INITSTK, 100, "test_mallocarray", 0), RESCHED_YES);
			break;
		case '1':
			ready(create((void *)printfreelist, INITSTK, 100, "test_printfreelist", 0), RESCHED_YES);
			break;
		default:
			break;
	}

	kprintf("\r\n===TEST END===\r\n");
	return;
}
