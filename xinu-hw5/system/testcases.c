/**
 * @file testcases.c
 * @provides testcases
 *
 * TA-BOT:MAILTO
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>

extern void main(int, char *);

int test_usernone(void) {
	kprintf("This is a test of ...");
	user_none();
	kprintf("user_none() syscall\r\n");

	return 0;
}

int test_userputc(void) {
	user_putc(0, 'H');
	user_putc(0, 'e');
	user_putc(0, 'l');
	user_putc(0, 'l');
	user_putc(0, 'o');
	user_putc(0, ' ');
	user_putc(0, 'W');
	user_putc(0, 'o');
	user_putc(0, 'r');
	user_putc(0, 'l');
	user_putc(0, 'd');
	user_putc(0, '!');
	user_putc(0, '\r');
	user_putc(0, '\n');

	return 0;
}

int test_usergetc(void) {
	char c;
	kprintf("Echo characters until 'X': ");
	while ((c = user_getc(0)) != 'X')
	{
		user_putc(0, c);
	}
	kprintf("\r\n");

	return 0;
}

int testmain(int argc, char **argv)
{
    int i = 0;
    kprintf("Hello XINU World!\r\n");

    for (i = 0; i < 10; i++)
    {
        kprintf("This is process %d\r\n", currpid);

        user_yield();
    }
    return 0;
}

void testbigargs(int a, int b, int c, int d, int e, int f, int g, int h)
{
    kprintf("Testing bigargs...\r\n");
    kprintf("a = 0x%08X\r\n", a);
    kprintf("b = 0x%08X\r\n", b);
    kprintf("c = 0x%08X\r\n", c);
    kprintf("d = 0x%08X\r\n", d);
    kprintf("e = 0x%08X\r\n", e);
    kprintf("f = 0x%08X\r\n", f);
    kprintf("g = 0x%08X\r\n", g);
    kprintf("h = 0x%08X\r\n", h);
}

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
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c;

    kprintf("0) Test user_none syscall\r\n");
    kprintf("1) Test user_getc syscall\r\n");
    kprintf("2) Test user_putc syscall\r\n");
    kprintf("3) Create three processes that test user_yield syscall\r\n");

    kprintf("===TEST BEGIN===\r\n");

    // TODO: Test your operating system!

    c = kgetc();
    switch (c)
    {
    case '0':
        ready(create((void *)test_usernone, INITSTK, "test_usernone", 0),
              RESCHED_YES);
		break;

    case '1':
        ready(create((void *)test_usergetc, INITSTK, "test_usergetc", 0),
              RESCHED_YES);
        break;

    case '2':
        ready(create((void *)test_userputc, INITSTK, "test_userputc", 0),
              RESCHED_YES);
		break;

    case '3':
        // Create three copies of a process, and let them play.
        ready(create((void *)testmain, INITSTK, "MAIN1", 2, 0, NULL),
              RESCHED_NO);
        ready(create((void *)testmain, INITSTK, "MAIN2", 2, 0, NULL),
              RESCHED_NO);
        ready(create((void *)testmain, INITSTK, "MAIN3", 2, 0, NULL),
              RESCHED_YES);
        while (numproc > 1)
            resched();
        break;

    default:
        break;
    }

    kprintf("\r\n===TEST END===\r\n");
    return;
}
