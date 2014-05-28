.origin 0
.entrypoint START

//
// SPECIFIEK VOOR PRU 1
//
// MOMENTEEL EEN LOOPLICHT OP LCD_DATA0 tm 4
//
//
//
//
//
//
//
//
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
//	r1	temporaries
//	r2	temporaries
//	r3	temporaries
//	r4	temporaries
//	r5:	ADC recording register
//	r6:	Sample copie
//
//	r7:	gedeeld geheugen	
//	r8:	masker om allen benodigde bits te krijgen
//	r9:	Aantal samples
//	r10	ADC pipeline latency count register
//
//
//	r30:	output pins
//	r31:	input pins
//
//	Eterne buffer op GPIO1[29]



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


#define ADCMASK 0x00000fff //masker van bits 0 -> 11
#define SHARED	0x00010000

//locatie in geheugen van cmd
#define CMD 0x00010000

//locatie in geheugen van samples (reserved ram)
#define RES_RAM 0x00010004

//hoeveelheid samples (voorlopig 100, nader te bepalen)
#define SAMPLES 100


.macro OPENBUFFER

.endm



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


	//zet macros in register zodat bewerkelijkheid beter is.
	MOV r6,	RES_RAM
	MOV r8,	ADCMASK
	MOV r9,	SAMPLES
	

	//test of het SHARED RAM werkt
	LBCO r0, CONST_DDR, 0, 12	//r1 t/m r3 worden gevuld met data uit het shared DDR RAM
		

	//TODO: open buffer wanneer niet open, vervolg dan hieronder

SET_LATENCY:				//Wordt gedaan om synchroon met ADC te lopen. Is niet nodig wanneer adc continue loopt
	MOV r10,	5			//lat reg = 5
LAT_SYNC:	
	WBC ADC_CLK				//wait until bit clear op ADC_CLK (falling edge)
	WBS ADC_CLK				//wait until bit set op ADC_CLK	(rising edge)
	SUB r10, r10, 1			//latency reg - 1
	QBNE LAT_SYNC, r10, 0	//jmp naar LAT_SYNC wanneer latency reg != 0
							//wanneer latency reg = 0, adc en pru gelijk qua registers. 

SAMPLE:
	WBC ADC_CLK				// om rising edge te detecteren, controlleer of clk laag is
	//Rising Edge komt nu
	WBS ADC_CLK
	
	// De onderstaande instructies zijn hetzelfde, dit is gedaan om de CLK-> D_OUT tijd op te vangen. Elke instructie is 5ns, er zijn dus 2 instructies nodig om de vertraging af te vangen.
	
	MOV r4, r31				
	MOV r4, r31				
	MOV r4, r31				//r4 is nu een realtime kopie van r31
	
	AND r6, r4, r8			//r6 bevat nu het and van r4 (de kopie van r31 (binaire waarde) en het bitmask. In principe alleen 
							//de adc databits staan nu in r6

	SBBO r6, r7, 0, 2		//kopieer 2bytes(16 bits) van r6(ADC_sample) naar r7 (RAM adress) zonder offset
	ADD	r7, r7, 4			//verhoog het adres met 1 32bit waarde (4 bytes)
	
	SUB  r9, r9, 1			// Trek 1 af van aan aantal te nemen samples.
	QBNE SAMPLE, r9, 0		// Jump naar SAMPLE zolang het aantal te nemen samples niet 0 is.
	
EINDE:
	
	//TODO: zet buffer weer in gesloten modus.
	MOV r1, 1				//temporary 1 = 1
	SBCO r1, CONST_PRUSHAREDRAM, 0, 4 // schrijf 0x01 naar shared ram, om aan te geven aan de main applicatie dat sample sequentie is afgerond.
	//in principe kan hier worden ingebakken dat pru niet moet worden uitgeschakeld, maar wel kan worden geresynced of toch exit
	
	
	EXIT:
    // Send notification to Host for program completion
    MOV       r31.b0, PRU1_ARM_INTERRUPT+16

    // Halt the processor
    HALT			
