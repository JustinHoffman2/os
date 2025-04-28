/**
 * @file     xsh_test.c
 * @provides xsh_test
 *
 */
/* Embedded XINU, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

void printFreeList(void);
/**
 * Shell command (test) is testing hook.
 * @param args array of arguments
 * @return OK for success, SYSERR for syntax error
 */
command xsh_test(int nargs, char *args[])
{
    int block = 0, i = 0;

    printFreeList();
    while (i < DISKBLOCKTOTAL)
    {
        block = sbGetBlock(supertab);
        printf("Block %d = %d\n", i, block);
        sbFreeBlock(supertab, block);
        printFreeList();
        if ((i < 5) || ((i > 54) && (i < 63)))
        {
            int c = getc(CONSOLE);
        }
    }
    
    return OK;
}

void printFreeList(void);
{
    int i;
    struct fbcnode *fbc = supertab->sb_freelst;
    while (fbc != NULL)
    {
        printf("Freelist Collector Nodes:\n");
        printf("Blk\t%d,\tcnt\t%d = ", fbc->fbc_blocknum, fbc->fbc_count);
        for (i = 0; i < 10 || i < fbc->fbc_count; i++)
        {
            printf("[%03d]", fbc->fbc_free[i]);
        }
        if (i == 10)
        {
            printf("...[%03d]\n", fbc->fbc_free[fbc->fbc_count - 1]);
        } 
        else
        {
            printf("\n");
        }
        fbc = fbc->fbc_next;
    }
}
