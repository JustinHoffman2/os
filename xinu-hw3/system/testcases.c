/**
 * @file testcases.c
 * @provides testcases
 *
 * Modified by:
 * Justin Hoffman
 * and
 * Luke Bondi
 */
/* Embedded XINU, Copyright (C) 2023.  All rights reserved. */

#include <xinu.h>

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c = 0;
    int a = 0;

    kprintf("===TEST BEGIN===\r\n");

    kprintf("\r\nInitial check of kcheckc(): %d", kcheckc());
    kprintf("\r\nPress any key: ");
    while (a != 1) {
	a = kcheckc();
    }
    kprintf("\r\nTrue check of kcheckc(): %d\r\n", a);
    
    kprintf("\r\nPress 'q' to quit");
    while (c != 'q') {
    c = kgetc();
   
    switch (c)
    {
	case 'b':
		kprintf("\r\nThe character %c was added to the ungetArray",c);
		kungetc(c);
		kprintf("\r\nThe character %c was taken out the ungetArray",kgetc());
		break;
	case 'c':
		kprintf("\r\nThe characters eee were added to ungetArray");
		kungetc('e');
		kungetc('e');
		kungetc('e');
	case '\n':
		kprintf("\r\n");
       
    default:
        kputc(c);
    }
    }

    kprintf("\r\n===TEST END===\r\n");
    return;
}
