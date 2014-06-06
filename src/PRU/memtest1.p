// *****************************************************************************/
// file:   PRU_memAcc_DDR_sharedRAM.p
//
// brief:  PRU Example to access DDR and PRU shared Memory.
//
//
//  (C) Copyright 2012, Texas Instruments, Inc
//
//  author     M. Watkins
//
//  version    0.1     Created
// *****************************************************************************/


.origin 0
.entrypoint MEMACCESS_DDR_PRUSHAREDRAM

#include "memtest.hp"

MEMACCESS_DDR_PRUSHAREDRAM:

    // Enable OCP master port
    LBCO      r0, CONST_PRUCFG, 4, 4
    CLR     r0, r0, 4         // Clear SYSCFG[STANDBY_INIT] to enable OCP master port
    SBCO      r0, CONST_PRUCFG, 4, 4

    // Configure the programmable pointer register for ??PRU1?? PRU0 by setting c28_pointer[15:0]
    
    //MOV     r0, 0x00000120 // field to 0x0120.  This will make C28 point to 0x00012000 (PRU0 shared RAM).
    MOV       r0, 0x00000100 // field to 0x0100.  This will make C28 point to 0x00010000 (PRU1 shared RAM).
    MOV       r1, CTPPR1_0
    ST32      r0, r1

    // Configure the programmable pointer register for ??PRU1?? PRU0 by setting c31_pointer[15:0]
    // field to 0x0010.  This will make C31 point to 0x80001000 (DDR memory).
    MOV     r0, 0x00100000
    MOV       r1, CTPPR1_1
    ST32      r0, r1

    //Load values from external DDR Memory into Registers R0/R1/R2
    LBCO      r0, CONST_DDR, 0, 12

    //Store values from read from the DDR memory into PRU shared RAM
    SBCO      r0, CONST_PRUSHAREDRAM, 0, 12

    // Send notification to Host for program completion
    MOV       r31.b0, PRU1_ARM_INTERRUPT+16

    // Halt the processor
    HALT


