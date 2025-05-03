/* sbGetBlock.c - sbGetBlock */
/* Copyright (C) 2007, Marquette University.  All rights reserved. */

#include <xinu.h>

/*------------------------------------------------------------------------
 * sbGetBlock - Get a free disk block from the superblock's free list.
 *------------------------------------------------------------------------
 */
devcall sbGetBlock(struct superblock *psuper)
{
    struct fbcnode *fbc, *fbc2;
    struct dirblock *swizzle;
    struct dentry *phw;
    int result, i;
    int diskfd;

    if (NULL == psuper)
    {
        return SYSERR;
    }
    phw = psuper->sb_disk;
    if (NULL == phw)
    {
        return SYSERR;
    }
    diskfd = phw - devtab;
    fbc = psuper->sb_freelst;
    if (NULL == fbc)
    {
        printf("sbGetFree() ERROR: Superblock free list is empty!\n");
        return SYSERR;
    }

    // Updating the free list must be mutually exclusive.
    wait(psuper->sb_freelock);
    // See if this first free block has some entries left.
    if (fbc->fbc_count > 0)
    {
        fbc->fbc_count--;
        result = fbc->fbc_free[fbc->fbc_count];
        fbc->fbc_free[fbc->fbc_count] = 0;

        // Update this free block record on disk.
        fbc2 = fbc->fbc_next;
        if (NULL == fbc->fbc_next)
        {
            fbc->fbc_next = 0;
        }
        else
        {
            fbc->fbc_next =
                (struct fbcnode *)fbc->fbc_next->fbc_blocknum;
        }
        seek(diskfd, fbc->fbc_blocknum);
        if (SYSERR == write(diskfd, fbc, sizeof(struct fbcnode)))
        {
            return SYSERR;
        }
        fbc->fbc_next = fbc2;
        if (!result)
            result = SYSERR;
    }
    else
        // The first free block list segment is empty.
    {
        // If the next list segment is null, then we're all out.
        if (NULL == fbc->fbc_next)
        {
            psuper->sb_freelst = NULL;
            swizzle = psuper->sb_dirlst;
            psuper->sb_dirlst = (struct dirblock *)swizzle->db_blocknum;
            seek(diskfd, psuper->sb_blocknum);
            if (SYSERR ==
                write(diskfd, psuper, sizeof(struct superblock)))
            {
                return SYSERR;
            }
            psuper->sb_dirlst = swizzle;
            result = fbc->fbc_blocknum;
            free(fbc);
            signal(psuper->sb_freelock);
            return result;
        }
        // Copy over contents of next free block list segment,
        //  and then give out the block containing that segment.
        fbc2 = fbc->fbc_next;
        result = fbc2->fbc_blocknum;
        for (i = fbc2->fbc_count - 1; i >= 0; i--)
        {
            fbc->fbc_free[i] = fbc2->fbc_free[i];
        }
        fbc->fbc_count = fbc2->fbc_count;
        fbc->fbc_next = fbc2->fbc_next;
        free(fbc2);

        // Update this free block record on disk.
        fbc2 = fbc->fbc_next;
        if (NULL == fbc->fbc_next)
        {
            fbc->fbc_next = 0;
        }
        else
        {
            fbc->fbc_next =
                (struct fbcnode *)fbc->fbc_next->fbc_blocknum;
        }
        seek(diskfd, fbc->fbc_blocknum);
        if (SYSERR == write(diskfd, fbc, sizeof(struct fbcnode)))
        {
            return SYSERR;
        }
        fbc->fbc_next = fbc2;

    }

    signal(psuper->sb_freelock);
    return result;
}
