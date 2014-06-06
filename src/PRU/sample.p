// *****************************************************************************/
// file:   sample.p
//
// brief:  Sampling external ADC.
//
//
//  (C) Copyright 2014,
//
//  author     S.van Schaick
//
//  version    0.1     Beta
// *****************************************************************************/
//////////////////////////////////////////////////
//
//	ADC:LTC2225
//	CLK->D_OUT Delay: min 1,4 ns    max 5,4 ns
//	ADC CLK		: 10MHz, 1 Cycle/instructie 100 ns
//	PRU CLK		:200MHz, 1 Cycle/instructie   5 ns
//	Pipeline latency:  5 ADC CLK Cycles
//
//////////////////////////////////////////////////
//
//	PRU CORE: 1
//	r0:	temporaties
//	r1:	temporaries
//	r2:	temporaries
//	r3:	temporaries
//	r4:	Commando's
//	r5:	ADC recording register
//	r6:	Sample copie
//
//	r7:	gedeeld geheugen	
//	r8:	masker om allen benodigde bits te krijgen
//	r9:	Aantal samples
//	r10	ADC pipeline latency count register
//
//	r11:	iets met commando
//	r12: 	offset shared voor commando
//
//	r30:	output pins
//	r31:	input pins
//
//	Eterne buffer op GPIO1[29]

.origin 0
.entrypoint BEGIN

#include "memtest.hp"
#include "pru_sander.hp"

#define ADC_MASK 0x00000fff //masker [11:0]

//locatie in geheugen van cmd tussen pru en am335x
#define CMD 0x00010000

//locatie in geheugen van samples (reserved ram)
#define RES_RAM 0x00010004

#define SAMPLES 100

#define ADC_CLK r31.t16

BEGIN:

    // Enable OCP master port
    LBCO      r0, CONST_PRUCFG, 4, 4
    CLR       r0, r0,		4         // Clear SYSCFG[STANDBY_INIT] to enable OCP master port
    SBCO      r0, CONST_PRUCFG, 4, 4

    // Configure the programmable pointer register for PRU1 by setting c28_pointer[15:0]
    
    //MOV     r0, 0x00000120 // field to 0x0120.  This will make C28 point to 0x00012000 (PRU0 shared RAM).
    MOV       r0, 0x00000100 // field to 0x0100.  This will make C28 point to 0x00010000 (PRU1 shared RAM).
    MOV       r1, CTPPR1_0
    ST32      r0, r1

    // Configure the programmable pointer register for PRU1 by setting c31_pointer[15:0]
    // field to 0x0010.  This will make C31 point to 0x80001000 (DDR memory).
    MOV       r0, 0x00100000
    MOV       r1, CTPPR1_1
    ST32      r0, r1

    // test memory match
    //Load values from external DDR Memory into Registers R0/R1/R2
    LBCO      r0, CONST_DDR, 0, 12

    //stuur gekopieerde gegevens terug.
    //Store values from read from the DDR memory into PRU shared RAM
    SBCO      r0, CONST_PRUSHAREDRAM, 0, 12

    //Zorg er voor dat buffer echt is uitgeschakeld
    BUFFER_DISABLE
    //zet macros in register zodat bewerkelijkheid beter is.
    MOV r7,	RES_RAM
    MOV r8,	ADC_MASK
    //MOV r9,	SAMPLES

    MOV r12, 	CONST_PRUSHAREDRAM
    ADD r12, r12, 3

WAIT_FI:
    MOV r11,	CMD

INS_LOOP:
    DELAY
    LBCO r13, r12, 0, 4
    //LBCO r13, CONST_PRUSHAREDRAM, 4, 4
    QBNE INS_LOOP, r13, 1 //blijf loop-en tot instructie op r13 1 wordt
    MOV r4, 0x01 //r4 = 1
    MOV r3, 0x00 //r3 = 0   
    SBCO r3, r12, 0, 4
    //SBCO r4, CONST_PRUSHAREDRAM, 4, 4
    SBCO r4, CONST_PRUSHAREDRAM, 0, 4
    //sharedram0 = 1, sharedram3 = 0
WAIT_FS:
    SHORT_DELAY
    LBCO r9, r12, 0, 4
    LBCO r13, CONST_PRUSHAREDRAM, 0, 4
    QBNE WAIT_FS, r13, 0x02

SAMPLING:
    BUFFER_ENABLE
    SBCO r3, r12, 0, 4 //maak sharedmem3 0 to be sure.

    //voor het nette zou de adc latency geset kunnen worden.
    //voor mij echt niet boeiend, dus ga dit niet doen.
SAMPLE:
    // om rising edge te detecteren, controlleer of clk laag is
    WBC ADC_CLK				
    //Rising Edge komt nu
    WBS ADC_CLK
	
    // De onderstaande instructies zijn hetzelfde, dit is gedaan om de CLK-> D_OUT tijd op te vangen.
    //Elke instructie is 5ns, er zijn dus 2 instructies nodig om de vertraging af te vangen.
	
    MOV r2, r31				
    MOV r2, r31				
    MOV r2, r31		//r2 is nu een realtime kopie van r31
	
    AND r6, r4, r8	//r6 bevat nu het and van r2 (de kopie van r31 (binaire waarde) en het bitmask. In principe alleen 
			//de adc databits staan nu in r6

    SBBO r6, r7, 0, 2	//kopieer 2bytes(16 bits) van r6(ADC_sample) naar r7 (RAM adress) zonder offset
    ADD	 r7, r7, 4	//verhoog het adres met 1 32bit waarde (4 bytes)
	
    SUB  r9, r9, 1	// Trek 1 af van aan aantal te nemen samples.
    QBNE SAMPLE, r9, 0	// Jump naar SAMPLE zolang het aantal te nemen samples niet 0 is.
    
    BUFFER_DISABLE

    MOV r1, 0x03 //r1 = 3
    SBCO r1, CONST_PRUSHAREDRAM, 0, 4

WAIT_FOR_EXIT:
    DELAY
    LBCO r1, CONST_PRUSHAREDRAM, 0, 4
    QBNE WAIT_FOR_EXIT, r1, 0x04

EXIT:
    // Send notification to Host for program completion
    MOV       r31.b0, PRU1_ARM_INTERRUPT+16
    // Halt the processor
    HALT


