/* fileDelete.c - fileDelete */
/* Copyright (C) 2008, Marquette University.  All rights reserved. */
/*                                                                 */
/* Modified by                                                     */
/*                                                                 */
/* and                                                             */
/*                                                                 */
/*                                                                 */

#include <xinu.h>

/*------------------------------------------------------------------------
 * fileDelete - Delete a file.
 *------------------------------------------------------------------------
 */
devcall fileDelete(int fd)
{
    // TODO: Unlink this file from the master directory index,
    //  and return its space to the free disk block list.
    //  Use the superblock's locks to guarantee mutually exclusive
    //  access to the directory index.
    

	if((supertab == NULL) || (filetab == NULL) ||
		(isbadfd(fd)) || filetab[fd].fn_state == FILE_FREE)
	{
		return SYSERR;
	}

	struct dentry *phw = supertab->sb_disk;
	int diskfd = phw - devtab;

	wait(supertab->sb_dirlock);

	if(supertab->sb_dirlst == NULL || filetab[fd].fn_state & FILE_OPEN)
	{
		signal(supertab->sb_dirlock);
		return SYSERR;
	}

	filetab[fd].fn_length = 0;
	filetab[fd].fn_cursor = 0;
	filetab[fd].fn_state = FILE_FREE;
	strcpy(filetab[fd].fn_name, "");
	free(filetab[fd].fn_data);
	filetab[fd].fn_data = NULL;
	if(sbFreeBlock(supertab, filetab[fd].fn_blocknum) == SYSERR)
	{
		signal(supertab->sb_dirlock);
		return SYSERR;
	}

	seek(diskfd, supertab->sb_dirlst->db_blocknum);
	int result = write(diskfd, supertab->sb_dirlst, sizeof(struct dirblock));

	signal(supertab->sb_dirlock);

	if(result == SYSERR)
	{
		return SYSERR;
	}

    return OK;
}
