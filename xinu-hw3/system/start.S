/**
 * @file start.S
 *
 * Initialization code for Embedded Xinu on the RISC-V
 *
 * Embedded Xinu, Copyright (C) 2024.  All rights reserved.
 *
 */
#include <riscv.h>

.section .init
	.globl _start

	/* _start:  Entry point of the Xinu kernel.  This will be the very first
	/* byte of the kernel image and on the will be loaded at the start address.  */

	.func _start
_start:
	/* Continue at reset handler. */
	j reset_handler
	.endfunc

.section .text
    .func clear_bss
clear_bss: // block starting symbol
	sw t0, 0(t2) // store word, dereferences t0, stores into t2+0    (t0 is temp/alt link reg) (t2 is a temp reg)
	addi t0, t0, 1 // adds value of t0, to 1 and stores in t0
	bltu t0, t1, clear_bss //branch if less then, if t0>t1 
	.endfunc

	/* reset_handler: Reset handler routine executed to start up the kernel,
	 * when the processor is reset, or (currently) when an unhandled
	 * exception occurs.  */
    .func reset_handler
reset_handler:

	// Add space for the stack
	la a1, _end
	li a3, 8192
	add sp, a1, a3


	la a4, memheap
	sw sp, 0(a4)

	call nulluser
	.endfunc