/**
 * @file interrupt.S
 */
/* Embedded Xinu, Copyright (C) 2013, 2024.  All rights reserved. */

#include <riscv.h>

.globl interrupt

/**
 * Entry point for Xinu's interrupt handler (RISC-V version). 
 */
interrupt:
	.func interrupt
    addi sp, sp, CONTEXT*-8

    sd sp, CTX_SP*8(sp)
    sd ra, CTX_RA*8(sp)
    sd gp, CTX_GP*8(sp)
    sd tp, CTX_TP*8(sp)
    sd t0, CTX_T0*8(sp)
    sd t1, CTX_T1*8(sp)
    sd t2, CTX_T2*8(sp)
    sd s0, CTX_S0*8(sp)
    sd s1, CTX_S1*8(sp)
    sd a0, CTX_A0*8(sp)
    sd a1, CTX_A1*8(sp)
    sd a2, CTX_A2*8(sp)
    sd a3, CTX_A3*8(sp)
    sd a4, CTX_A4*8(sp)
    sd a5, CTX_A5*8(sp)
    sd a6, CTX_A6*8(sp)
    sd a7, CTX_A7*8(sp)
    sd s2, CTX_S2*8(sp)
    sd s3, CTX_S3*8(sp)
    sd s4, CTX_S4*8(sp)
    sd s5, CTX_S5*8(sp)
    sd s6, CTX_S6*8(sp)
    sd s7, CTX_S7*8(sp)
    sd s8, CTX_S8*8(sp)
    sd s9, CTX_S9*8(sp)
    sd s10, CTX_S10*8(sp)
    sd s11, CTX_S11*8(sp)
    sd t3, CTX_T3*8(sp)
    sd t4, CTX_T4*8(sp)
    sd t5, CTX_T5*8(sp)
    sd t6, CTX_T6*8(sp)
    
    csrr a0, scause 
    csrr a1, stval
    mv a2, sp
    csrr a3, sepc
    call dispatch

    ld sp, CTX_SP*8(sp)
    ld ra, CTX_RA*8(sp)
    ld gp, CTX_GP*8(sp)
    ld tp, CTX_TP*8(sp)
    ld t0, CTX_T0*8(sp)
    ld t1, CTX_T1*8(sp)
    ld t2, CTX_T2*8(sp)
    ld s0, CTX_S0*8(sp)
    ld s1, CTX_S1*8(sp)
    ld a0, CTX_A0*8(sp)
    ld a1, CTX_A1*8(sp)
    ld a2, CTX_A2*8(sp)
    ld a3, CTX_A3*8(sp)
    ld a4, CTX_A4*8(sp)
    ld a5, CTX_A5*8(sp)
    ld a6, CTX_A6*8(sp)
    ld a7, CTX_A7*8(sp)
    ld s2, CTX_S2*8(sp)
    ld s3, CTX_S3*8(sp)
    ld s4, CTX_S4*8(sp)
    ld s5, CTX_S5*8(sp)
    ld s6, CTX_S6*8(sp)
    ld s7, CTX_S7*8(sp)
    ld s8, CTX_S8*8(sp)
    ld s9, CTX_S9*8(sp)
    ld s10, CTX_S10*8(sp)
    ld s11, CTX_S11*8(sp)
    ld t3, CTX_T3*8(sp)
    ld t4, CTX_T4*8(sp)
    ld t5, CTX_T5*8(sp)
    ld t6, CTX_T6*8(sp)

    addi sp, sp, CONTEXT*8
    
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

