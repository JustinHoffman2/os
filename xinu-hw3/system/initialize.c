/**
 * @file initialize.c
 * The system begins intializing after the C environment has been
 * established.  After intialization, the null process remains always in
 * a ready or running state.
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>

/* Function prototypes */
static int sysinit(void);       /* intializes system structures          */
static void welcome(void);      /* Print inital O/S data                 */
process main(void); /* main is the first process created     */


/* Params set by startup.S */
void *memheap;                  /* Bottom of heap (top of O/S stack)     */
ulong cpuid;                    /* Processor id                          */

struct platform platform;       /* Platform specific configuration       */


void nulluser(void)
{
    /* Platform-specific initialization */
    platforminit();

    /* General initialization  */
    sysinit();

    /* Standard Embedded Xinu processor and memory info */
    welcome();

    /* Call the main program */
    main();

    /* null process has nothing else to do but cannot exit  */
    while (1)
    {

    }
}

static void welcome(void)
{
    kprintf(VERSION);
    kprintf("\r\n\r\n");

    /* Output detected platform. */
    kprintf("Detected platform as: [%s] %s (ver %d) RV%dI %s \r\n\r\n",
            platform.manufacturer, platform.family, platform.revision,
            platform.architecture, platform.extensions);

    /* Output Xinu memory layout */
    kprintf("%10d bytes physical memory.\r\n",
            (ulong)platform.maxaddr - (ulong)platform.minaddr);
    kprintf("           [0x%016lX to 0x%016lX]\r\n",
            (ulong)platform.minaddr, (ulong)(platform.maxaddr - 1));

    kprintf("%10lu bytes reserved system area.\r\n",
            (ulong)_start - (ulong)platform.minaddr);
    kprintf("           [0x%016lX to 0x%016lX]\r\n",
            (ulong)platform.minaddr, (ulong)_start - 1);

    kprintf("%10d bytes Xinu code.\r\n", (ulong)&_end - (ulong)_start);
    kprintf("           [0x%016lX to 0x%016lX]\r\n",
            (ulong)_start, (ulong)&_end - 1);

    kprintf("%10d bytes kernel stack space.\r\n",
            (ulong)memheap - (ulong)&_end);
    kprintf("           [0x%016lX to 0x%016lX]\r\n",
            (ulong)&_end, (ulong)memheap - 1);
    kprintf("%10d bytes heap space.\r\n",
            (ulong)platform.maxaddr - (ulong)memheap);
    kprintf("           [0x%016lX to 0x%016lX]\r\n\r\n",
            (ulong)memheap, (ulong)platform.maxaddr - 1);

    kprintf("\r\n");
}

/**
 * Intializes all Xinu data structures and devices.
 * @return OK if everything is initialized successfully
 */
static int sysinit(void)
{
    // Counter for unget buffer
    bufp = 0;

    return OK;
}
