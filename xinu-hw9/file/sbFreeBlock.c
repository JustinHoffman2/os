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

	if(fbc == NULL) //case 1
	{
		struct fbcnode *newfbc = (fbcnode *)malloc(sizeof(fbcnode));
		newfbc->fbc_blocknum = block;
		newfbc->fbc_count = 0;
		newfbc->fbc_next = NULL;
		psuper->sb_freelist = newfbc;

		//call swizzle somewhere
		signal(psuper->sb_freelock);
		return OK;
		//signal the lock and then return 
		//for all cases do steps 1 and 2 
		//need to set up the super block 
		//other than that set up very similar to case 2 
	}

	while(fbc->next != NULL)
	{
		fbc = fbc->next;
	}

	if(fbc->fbc_count == FREEBLOCKMAX || fbc->fbc_count == 0) //case 2
	{
		//malloc to grab another block of space freeblock struct
		struct fbcnode *newfbc = (fbcnode *)malloc(sizeof(fbcnode));
		newfbc->fbc_blocknum = block;
		newfbc->fbc_count = 0;
		newfbc->fbc_next = NULL;
		fbc->next = newfbc;

		//call swizzle
		signal(psuper->sb_freelock);
		return OK;
	}

    return SYSERR;
}

swizzle(struct fbcnode fbc) {
	
}
