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
	kprintf("\tAddress:\r\n");
	while(block != NULL){
		kprintf("\t\t0x%lX\r\n",block);
		block = block->next;
	}
	kprintf("\tLength: %ld\r\n", head->length);
	kprintf("\tBound: %lX\r\n", head->bound);
}

void mallocarray(void) {
	kprintf("Free list before malloc:\r\n");
	printfreelist();
	int *array;
	int n = 10;
	array = (int*) malloc(n*sizeof(int));
	kprintf("Free list after malloc:\r\n");
	printfreelist();
	for(int i = 0; i < n; i++){
		array[i] = i + 1;
		kprintf("%d\t%lx\r\n", array[i], &array[i]);
	}
	kprintf("Free list after free:\r\n");
	free(array);
	printfreelist();
}

void limittest(void) {
	kprintf("More malloc test\r\n");
	kprintf("Free list before array malloc:\r\n");
        printfreelist();
        
	int *array;
        array = (int*) malloc(1000*sizeof(int));
        kprintf("Free list after array malloc:\r\n");
        printfreelist();
	
	char *str;
	str = (char*)malloc(10*sizeof(char));
	kprintf("Free list after char malloc:\r\n");
	printfreelist();
	
	kprintf("Free list after char free:\r\n");
	free(str);
	printfreelist();

        kprintf("Free list after array free:\r\n");
        free(array);
        printfreelist();
}

void compaction(void) {
	kprintf("Compaction Test\r\n");
        kprintf("Before malloc:\r\n");
        printfreelist();
        char *str1;
	char *str2;
	char *str3;
        str1 = (char *)malloc(50*sizeof(char));
        str2 = (char *)malloc(20*sizeof(char));
	str3 = (char *)malloc(20*sizeof(char));
	kprintf("After malloc:\r\n");
	printfreelist();

        kprintf("After str2 free:\r\n");
	free(str2);
	printfreelist();

        kprintf("After str1 free:\r\n");
        free(str1);
        printfreelist();

	kprintf("After str3 free:\r\n");
	free(str3);
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
	kprintf("2) limit test\r\n");
	kprintf("3) freemem compaction\r\n");
	kprintf("\r\n");
	c = kgetc();
	switch (c)
	{
		// Test getmem & freemem
		case '0':
			ready(create((void *)mallocarray, INITSTK, 100, "test_mallocarray", 0), RESCHED_YES);
			break;
		// Free List printing
		case '1':
			ready(create((void *)printfreelist, INITSTK, 100, "test_printfreelist", 0), RESCHED_YES);
			break;
		// Allocate & free as much memory as possible
		case '2':
			ready(create((void *)limittest, INITSTK, 100, "test_limittest", 0), RESCHED_YES);
			break;
		// Check freemem compaction
		case '3':
			ready(create((void *)compaction, INITSTK, 100, "test_compaction", 0), RESCHED_YES);
			break;
		default:
			break;
	}

	kprintf("\r\n===TEST END===\r\n");
	return;
}
