/* sbFreeBlock.c - sbFreeBlock */
/* Copyright (C) 2008, Marquette University.  All rights reserved. */
/*                                                                 */
/* Modified by                                                     */
/* Justin Hoffman                                                  */
/* and                                                             */
/* Luke Bondi                                                      */
/*                                                                 */

#include <xinu.h>

/*------------------------------------------------------------------------
 * sbFreeBlock - Add a block back into the free list of disk blocks.
 *------------------------------------------------------------------------
 */
int swizzle(struct fbcnode *, int);

devcall sbFreeBlock(struct superblock *psuper, int block)
{
    // TODO: Add the block back into the filesystem's list of
    //  free blocks.  Use the superblock's locks to guarantee
    //  mutually exclusive access to the free list, and write
    //  the changed free list segment(s) back to disk.
    //
	int diskfd;
	struct dentry *phw;
	
	if (psuper == NULL)
	{
		return SYSERR;
	}
	phw = psuper->sb_disk;
	if (phw == NULL)
	{
		return SYSERR;
	}

	if (block <= 0 && block > DISKBLOCKTOTAL)
	{
	       return SYSERR;
	}

	diskfd = phw - devtab;
	wait(psuper->sb_freelock);
	struct fbcnode *fbc = psuper->sb_freelst;

	if (fbc == NULL) // Case 1
	{
		struct dirblock *swizzleSB;
		struct fbcnode *newfbc = (struct fbcnode *)malloc(sizeof(struct fbcnode));
		newfbc->fbc_blocknum = block;
		newfbc->fbc_count = 0;
		newfbc->fbc_next = NULL;
		psuper->sb_freelst = newfbc;

		if (swizzle(newfbc, diskfd) == NULL)
		{
			return SYSERR;
		}
		// Write superblock to disk
		swizzleSB = psuper->sb_dirlst;
		psuper->sb_dirlst = (struct dirblock *)swizzleSB->db_blocknum;
		seek(diskfd, psuper->sb_blocknum);
		if (write(diskfd, psuper, sizeof(struct superblock)) == SYSERR)
		{
			return SYSERR;
		}
		psuper->sb_dirlst = swizzleSB;

		signal(psuper->sb_freelock);
		return OK; 
	}

	while (fbc->fbc_next != NULL)
	{
		fbc = fbc->fbc_next;
	}

	if (fbc->fbc_count == FREEBLOCKMAX || fbc->fbc_count == 0) // Case 2
	{
		// Malloc to get enough space to make a new collector node
		struct fbcnode *newfbc = (struct fbcnode *)malloc(sizeof(struct fbcnode));
		newfbc->fbc_blocknum = block;
		newfbc->fbc_count = 0;
		newfbc->fbc_next = NULL;
		fbc->fbc_next = newfbc;

		if (swizzle(fbc, diskfd) == NULL || swizzle(newfbc, diskfd) == NULL)
		{
			return SYSERR;
		}

		signal(psuper->sb_freelock);
		return OK;
	}
	
	// Case 3
	fbc->fbc_free[fbc->fbc_count] = block;
	fbc->fbc_count++;
	if (swizzle(fbc, diskfd) == NULL)
	{
		return SYSERR;
	}

	signal(psuper->sb_freelock);
	return OK;
}

// Writes fbc to disk
int swizzle(struct fbcnode *fbc, int diskfd) {
	struct fbcnode *fbc2 = fbc->fbc_next;
	if (fbc2 == NULL)
	{
		fbc->fbc_next = 0;
	}
	else
	{
		fbc->fbc_next = (struct fbcnode *)fbc2->fbc_blocknum;
	}
	seek(diskfd, fbc->fbc_blocknum);
	if (write(diskfd, fbc, sizeof(struct fbcnode)) == SYSERR)
	{
		return SYSERR;
	}
	fbc->fbc_next = fbc2;
	return OK;
	
}
