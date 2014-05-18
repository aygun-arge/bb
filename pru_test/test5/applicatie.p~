.origin 0
.entrypoint START

//
// SPECIFIEK VOOR PRU 1
//
// MOMENTEEL EEN LOOPLICHT OP LCD_DATA0 tm 4
//




#include "PRU_memAccess_DDR_PRUsharedRAM.hp"
#include "pru_sander.hp"

#define DELAYWAARDE 0x00a00000

#define D0 r30.t0
#define D1 r30.t1
#define D2 r30.t2
#define D3 r30.t3
#define D4 r30.t4


#define ADC_D0  r31.t0
#define ADC_D1  r31.t1
#define ADC_D2  r31.t2
#define ADC_D3  r31.t3
#define ADC_D4  r31.t4
#define ADC_D5  r31.t5
#define ADC_D6  r31.t6
#define ADC_D7  r31.t7
#define ADC_D8  r31.t8
#define ADC_D9  r31.t9
#define ADC_D10 r31.t10
#define ADC_D11 r31.t11
#define ADC_CLK r31.t16

#define BITMASK 0x00000fff //masker van bits 0 -> 11




.macro DELAY
DELAY:	
	MOV r1, 20000000
DEL:
	SUB r1, r1, 1
	QBNE DEL, r1, 0
.endm

START:
    //enable ocp master
    LBCO    r0, CONST_PRUCFG, 4, 4
    CLR     r0, r0,           4         // Clear SYSCFG[STANDBY_INIT] to enable OCP master port
    SBCO    r0, CONST_PRUCFG, 4, 4


    // Configure the programmable pointer register for PRU1 by setting c28_pointer[15:0]
    // field to 0x0120.  This will make C28 point to 0x00012000 (PRU shared RAM).

    MOV       r0, 0x00000120
    MOV       r1, CTPPR_0
    ST32      r0, r1
	
	// Configure the programmable pointer register for PRU1 by setting c31_pointer[15:0]

    // field to 0x0010.  This will make C31 point to 0x80001000 (DDR memory).

    MOV	      r0, 0x00100000
    MOV       r1, CTPPR_1
    ST32      r0, r1

	MOV r6, 10 			//r6 = 10 om 10x te loop-en
GA:
	SET D0
 	DELAY
	CLR D0
	SET D1
	DELAY
	CLR D1
	SET D2
	DELAY
	CLR D2
	SET D3
	DELAY
	CLR D3
	SET D4
	DELAY
	CLR D4
	SUB r6, r6 , 1
	QBNE GA, r6, 0			// jump naar GA wanneer r6 = niet 0


SAMPLE:




EXIT:
    // Send notification to Host for program completion
    MOV       r31.b0, PRU1_ARM_INTERRUPT+16

    // Halt the processor
    HALT			
