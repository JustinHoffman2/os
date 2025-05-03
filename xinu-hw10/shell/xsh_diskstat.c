/**
 * @file     xsh_diskstat.c
 * @provides xsh_diskstat
 *
 */
/* Embedded XINU, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

void printFreeList(void)
{
    int i;
    struct fbcnode *fbc = NULL;
    if ((NULL == supertab) || (NULL == (fbc = supertab->sb_freelst)))
    {
        printf("Freelst: null\n");
        return;
    }

    printf("Freelst collector nodes:\n");
    while (fbc)
    {
        printf("Blk %3d, cnt %3d = ", fbc->fbc_blocknum, fbc->fbc_count);
        for (i = 0; (i < fbc->fbc_count) && (i < 10); i++)
        {
            printf("[%03d]", fbc->fbc_free[i]);
        }
        if (fbc->fbc_count >= 10)
        {
            printf("...[%03d]", fbc->fbc_free[fbc->fbc_count - 1]);
        }
        printf("\n");
        fbc = fbc->fbc_next;
    }
}

void printDirectory(void)
{
    int i = 0;

    printf("Directory block:\n");
    printf("entry length cursor block  state  name\n");
    printf("----- ------ ------ ------ ------ --------\n");
    for (i = 0; i < DIRENTRIES; i++)
    {
        if (filetab[i].fn_state)
        {
            printf("   %2d    %3d    %3d    %3d ",
                   i, filetab[i].fn_length, filetab[i].fn_cursor,
                   filetab[i].fn_blocknum);
            if (filetab[i].fn_state & FILE_USED)
            {
                printf("U");
            }
            else
            {
                printf(" ");
            }
            if (filetab[i].fn_state & FILE_OPEN)
            {
                printf("O");
            }
            else
            {
                printf(" ");
            }
            if (filetab[i].fn_state & FILE_DIRTY)
            {
                printf("D");
            }
            else
            {
                printf(" ");
            }
            printf("    %s\n", filetab[i].fn_name);
        }
    }
}

/**
 * Shell command (diskstat) prints Xinu disk status.
 * @param args array of arguments
 * @return OK for success, SYSERR for syntax error
 */
command xsh_diskstat(int nargs, char *args[])
{
    /* Output help, if '--help' argument was supplied */
    if ((nargs == 2 && strncmp(args[1], "--help", 6) == 0) || (nargs > 1))
    {
        printf("Usage: diskstat\n");
        printf("Displays status of disk.\n");
        printf("\t--help\t display this help and exit\n");

        return OK;
    }

    printf("\n");
    printDirectory();
    printf("\n");
    printFreeList();
    printf("\n");

    return OK;
}
