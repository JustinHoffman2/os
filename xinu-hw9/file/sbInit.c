/* sbInit.c - sbInit */
/* Copyright (C) 2007, Marquette University.  All rights reserved. */

#include <xinu.h>

/*------------------------------------------------------------------------
 * sbInit - Initialize a fresh filesystem superblock
 *------------------------------------------------------------------------
 */
devcall sbInit(struct superblock *psuper, int diskfd)
{
    struct fbcnode *fbc = NULL;
    struct dirblock *dirblk = NULL;
    int i, j, curblock;

    printf("Initializing filesystem superblock.\n");
    // Initialize disk.
    psuper->sb_blocknum = SUPERBLOCKNUM;
    psuper->sb_blocktotal = DISKBLOCKTOTAL;
    psuper->sb_dirlst = (struct dirblock *)(SUPERBLOCKNUM + 1);
    psuper->sb_freelst = (struct fbcnode *)(SUPERBLOCKNUM + 2);
    // Write superblock to disk block 0.
    write(diskfd, psuper, sizeof(struct superblock));

    // Initialize list of free disk blocks.
    curblock = (int)psuper->sb_freelst;
    i = curblock + 1;
    j = 0;
    fbc = psuper->sb_freelst = malloc(sizeof(struct fbcnode));
    if (NULL == fbc)
    {
        return SYSERR;
    }
    do
    {
        fbc->fbc_free[j] = i;
        i++;
        j++;
        if (j == FREEBLOCKMAX)
        {
            fbc->fbc_blocknum = curblock;
            fbc->fbc_count = j;
            fbc->fbc_next = (struct fbcnode *)i;
            seek(diskfd, curblock);
            write(diskfd, fbc, sizeof(struct fbcnode));
            curblock = i;
            i = curblock + 1;
            j = 0;
            fbc = fbc->fbc_next = malloc(sizeof(struct fbcnode));
            if (NULL == fbc)
            {
                return SYSERR;
            }
        }
    }
    while (i < DISKBLOCKTOTAL);
    // Remainder of the free list.
    if (DISKBLOCKTOTAL != curblock)
    {
        fbc->fbc_blocknum = curblock;
        fbc->fbc_count = j;
        fbc->fbc_next = NULL;
        seek(diskfd, curblock);
        write(diskfd, fbc, sizeof(struct fbcnode));
    }


    // Initialize directory structure
    curblock = (int)psuper->sb_dirlst;
    psuper->sb_dirlst = dirblk = malloc(sizeof(struct dirblock));
    if (NULL == dirblk)
    {
        return SYSERR;
    }
    dirblk->db_blocknum = curblock;
    for (i = 0; i < DIRENTRIES; i++)
    {
        dirblk->db_fnodes[i].fn_state = FILE_FREE;
    }
    seek(diskfd, curblock);
    write(diskfd, dirblk, sizeof(struct dirblock));

    filetab = dirblk->db_fnodes;
    return OK;
}
