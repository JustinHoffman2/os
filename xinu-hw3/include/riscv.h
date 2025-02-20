/**
 * @file riscv.h
 * 
 * Contains constants for the RISC-V architecture.  
 * NOTE: This file is included in assembly, thus it can only contain constants
 */
/* Embedded Xinu, Copyright (C) 2024.  All rights reserved. */

#ifndef  _RISCV_H_
#define  _RISCV_H_

#define RISCV_MSTATUS_SUM       1L<<18
#define RISCV_MSTATUS_MEI_BIT   1<<3    /* IRQs globally disabled on all privilege levels when set to 1. */
#define RISCV_MPP_TO_S_MODE     1<<11   /* Set the previous mode to S-mode so XINU can switch to S-Mode when completed. */
#define RISCV_SPP_TO_U_MODE     1<<8    /* Set the previous mode to U-mode so XINU can switch to U-Mode when completed. */
#define RISCV_SIE_ENABLE        1<<1
#define RISCV_SIE_DISABLE       0<<1
#define RISCV_MDELEG_ALL_S_MODE 0xFFFFFFFFFFFFFFFF

#define RISCV_SIE_SEIE 1<<9
#define RISCV_SIE_STIE 1<<5
#define RISCV_SIE_SSIE 1<<1

#define RISCV_ENABLE_ALL_SMODE_INTR (RISCV_SIE_SEIE | RISCV_SIE_STIE | RISCV_SIE_SSIE)
#define RISCV_MAX_ADDR 0x3FFFFFFFFFFFFFull
#define RISCV_ALL_PERM 0xF

#define MAXVIRTADDR		0x4000000000        //(1L << 38)
#define INTERRUPTADDR   0x3FFFFFF000    // truncpage((MAXVIRTADDR - PAGE_SIZE))
#define SWAPAREAADDR    0x3FFFFFE000    // truncpage((MAXVIRTADDR - PAGE_SIZE))
#define PROCSTACKADDR   0x3FFFFFD000    // truncpage((MAXVIRTADDR - PAGE_SIZE - PAGE_SIZE))

#define WATCHDOG_CONF	0x00020500B4

#endif                          /* _RISCV_H_ */
