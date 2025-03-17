/**
 * @file resched.c
 * @provides resched
 *
 * COSC 3250 Assignment 6
 */
/* Embedded XINU, Copyright (C) 2008,2024.  All rights reserved. */

#include <xinu.h>

extern void ctxsw(void *, void *);
pid_typ lottery(void);
/**
 * Reschedule processor to next ready process.
 * Upon entry, currpid gives current process id.  Proctab[currpid].pstate 
 * gives correct NEXT state for current process if other than PRREADY.
 * @return OK when the process is context switched back
 */
syscall resched(void)
{
    pcb *oldproc;               /* pointer to old process entry */
    pcb *newproc;               /* pointer to new process entry */

    oldproc = &proctab[currpid];

    /* place current process at end of ready queue */
    if (PRCURR == oldproc->state)
    {
        oldproc->state = PRREADY;
        enqueue(currpid, readylist);
    }

    /**
     * We recommend you use a helper function for the following:
     * TODO: Get the total number of tickets from all processes that are in
     * current and ready states.
     * Use random() function to pick a random ticket. 
     * Traverse through the process table to identify which proccess has the
     * random ticket value.  Remove process from queue.
     * Set currpid to the new process.
     */
	
    currpid = remove(lottery());

   // currpid = dequeue(readylist);
    newproc = &proctab[currpid];
    newproc->state = PRCURR;    /* mark it currently running    */

#if PREEMPT
    preempt = QUANTUM;
#endif

    // Shows which processes the scheduler switches to
    kprintf("[%d %d]", oldproc-proctab, newproc-proctab); // Added from Lecture
    
    ctxsw(&oldproc->ctx, &newproc->ctx);

    /* The OLD process returns here when resumed. */
    return OK;
}

pid_typ lottery(void) {
	int winner;
	pid_typ pid;
	pcb *process;
	int counter = 0;
	unsigned int total = 0;

	// Finds all of the tickets of the current and ready processes
	pid = firstid(readylist);
	while (pid != EMPTY) {
		process = &proctab[pid];
		total += process->tickets;
		pid = queuetab[pid].next;
	}
	winner = random(total);
	pid = firstid(readylist);
	while (pid != EMPTY) {
		process = &proctab[pid];
		counter += process->tickets;
		if (counter > winner)
			break;
		pid = queuetab[pid].next;
	}
	return pid;
}
