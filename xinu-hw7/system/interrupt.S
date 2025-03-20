/**
 * @file interrupt.S
 */
/* Embedded Xinu, Copyright (C) 2013, 2024.  All rights reserved. */

#include <riscv.h>  /* Needed for ARM_MODE_SYS definition.  */

.section .interruptsec
    .align 4
.globl interrupt

/**
 * Entry point for Xinu's interrupt handler (RISC-V version). 
 */
interrupt:
	.func interrupt

    csrw sscratch, a0

    li a0, SWAPAREAVADDR

    sd t0, CTX_T0*8(a0)
    mv t0, a0
    csrr a0, sscratch

    // Store process context in designated swap area
    sd sp, CTX_SP*8(t0)
    sd ra, CTX_RA*8(t0)
    sd gp, CTX_GP*8(t0)
    sd tp, CTX_TP*8(t0)
    sd t1, CTX_T1*8(t0)
    sd t2, CTX_T2*8(t0)
    sd s0, CTX_S0*8(t0)
    sd s1, CTX_S1*8(t0)
    sd a0, CTX_A0*8(t0)
    sd a1, CTX_A1*8(t0)
    sd a2, CTX_A2*8(t0)
    sd a3, CTX_A3*8(t0)
    sd a4, CTX_A4*8(t0)
    sd a5, CTX_A5*8(t0)
    sd a6, CTX_A6*8(t0)
    sd a7, CTX_A7*8(t0)
    sd s2, CTX_S2*8(t0)
    sd s3, CTX_S3*8(t0)
    sd s4, CTX_S4*8(t0)
    sd s5, CTX_S5*8(t0)
    sd s6, CTX_S6*8(t0)
    sd s7, CTX_S7*8(t0)
    sd s8, CTX_S8*8(t0)
    sd s9, CTX_S9*8(t0)
    sd s10, CTX_S10*8(t0)
    sd s11, CTX_S11*8(t0)
    sd t3, CTX_T3*8(t0)
    sd t4, CTX_T4*8(t0)
    sd t5, CTX_T5*8(t0)
    sd t6, CTX_T6*8(t0)
   
    // Load kernel page table
    ld a0, CTX_KERNSATP*8(t0)	
    ld sp, CTX_KERNSP*8(t0)
    // call set_satp
    sfence.vma zero, zero
    csrw satp, a0
    sfence.vma zero, zero

    csrr a0, scause 
    csrr a1, stval
    mv   a2, t0
    csrr a3, sepc
    call dispatch
    // Load process page table
    //call set_satp
    sfence.vma zero, zero
    csrw satp, a0
    sfence.vma zero, zero

    // Restore process context from swap area
    li a0, SWAPAREAVADDR
    ld t0, CTX_T0*8(a0)
    csrw sscratch, t0
    mv t0, a0

    ld sp, CTX_SP*8(t0)
    ld ra, CTX_RA*8(t0)
    ld gp, CTX_GP*8(t0)
    ld tp, CTX_TP*8(t0)
    ld t1, CTX_T1*8(t0)
    ld t2, CTX_T2*8(t0)
    ld s0, CTX_S0*8(t0)
    ld s1, CTX_S1*8(t0)
    ld a0, CTX_A0*8(t0)
    ld a1, CTX_A1*8(t0)
    ld a2, CTX_A2*8(t0)
    ld a3, CTX_A3*8(t0)
    ld a4, CTX_A4*8(t0)
    ld a5, CTX_A5*8(t0)
    ld a6, CTX_A6*8(t0)
    ld a7, CTX_A7*8(t0)
    ld s2, CTX_S2*8(t0)
    ld s3, CTX_S3*8(t0)
    ld s4, CTX_S4*8(t0)
    ld s5, CTX_S5*8(t0)
    ld s6, CTX_S6*8(t0)
    ld s7, CTX_S7*8(t0)
    ld s8, CTX_S8*8(t0)
    ld s9, CTX_S9*8(t0)
    ld s10, CTX_S10*8(t0)
    ld s11, CTX_S11*8(t0)
    ld t3, CTX_T3*8(t0)
    ld t4, CTX_T4*8(t0)
    ld t5, CTX_T5*8(t0)
    ld t6, CTX_T6*8(t0)

    csrr t0, sscratch

    sret

.endfunc

.globl switchmode
    .func switchmode
switchmode:
    csrr a1, sstatus
    andi a1, a1, SSTATUS_PRIV_MODE_BIT
    beq a0, a1, return

    li t3, SSTATUS_U_MODE
    beq a1, t3, set_smode

    li t4, SSTATUS_S_MODE
    csrc sstatus, t4
    ret 
set_smode:
    li t4, SSTATUS_S_MODE
    csrs sstatus, t4
    ret 
return:
    ret
.endfunc

