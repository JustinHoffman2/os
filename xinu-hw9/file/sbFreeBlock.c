/* sbFreeBlock.c - sbFreeBlock */
/* Copyright (C) 2008, Marquette University.  All rights reserved. */
/*                                                                 */
/* Modified by                                                     */
/*                                                                 */
/* and                                                             */
/*                                                                 */
/*                                                                 */

#include <xinu.h>

/*------------------------------------------------------------------------
 * sbFreeBlock - Add a block back into the free list of disk blocks.
 *------------------------------------------------------------------------
 */
devcall sbFreeBlock(struct superblock *psuper, int block)
{
    // TODO: Add the block back into the filesystem's list of
    //  free blocks.  Use the superblock's locks to guarantee
    //  mutually exclusive access to the free list, and write
    //  the changed free list segment(s) back to disk.
    //

	int diskfd;
	struct dentry *phw;
	if(superblock == NULL)
	{
		return SYSERR;
	}
	phw = psuper->sb_disk;
	if(phw != NULL)
	{
		return SYSERR;
	}

	if(block > 0 && <= DISKBLOCKTOTAL)
	{
	       return SYSERR;
	}	       
	diskfd = phw - devtab; //devtab is device table
	wait(psuper->sb_freelock);
	struct fbcnode *fbc = psuper->sb_freelist;

    return SYSERR;
}
