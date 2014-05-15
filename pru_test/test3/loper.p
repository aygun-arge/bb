.origin 0
.entrypoint START

#define GPIO1 0x4804c000
#define GPIO_CLEARDATAOUT 0x190
#define GPIO_SETDATAOUT 0x194
#define DELAYWAARDE 0x00a00000


#define OFFSET_USR3 1<<24
#define OFFSET_USR2 1<<23
#define OFFSET_USR1 1<<22
#define OFFSET_USR0 1<<21

.macro USR3_ON
	MOV r2, OFFSET_USR3
	MOV r3, GPIO1 | GPIO_SETDATAOUT // maakt r3 GPIO1 bitwise or setdata = 4804c194
	SBBO r2, r3, 0, 4		// copy 4 bytes van r2 naar r3 offset 0
.endm

.macro USR2_ON
	MOV r2, OFFSET_USR2
	MOV r3, GPIO1 | GPIO_SETDATAOUT
	SBBO r2, r3, 0, 4
.endm

.macro USR1_ON
	MOV r2, OFFSET_USR1
	MOV r3, GPIO1 | GPIO_SETDATAOUT
	SBBO r2, r3, 0, 4
.endm

.macro USR0_ON
	MOV r2, OFFSET_USR0
	MOV r3, GPIO1 | GPIO_SETDATAOUT
	SBBO r2, r3, 0, 4
.endm

.macro USR3_OFF
	MOV r2, OFFSET_USR3
	MOV r3, GPIO1 | GPIO_CLEARDATAOUT
	SBBO r2, r3, 0, 4
.endm
		
.macro USR2_OFF
	MOV r2, OFFSET_USR2
	MOV r3, GPIO1 | GPIO_CLEARDATAOUT
	SBBO r2, r3, 0, 4
.endm

.macro USR1_OFF
	MOV r2, OFFSET_USR1
	MOV r3, GPIO1 | GPIO_CLEARDATAOUT
	SBBO r2, r3, 0, 4
.endm

.macro USR0_OFF
	MOV r2, OFFSET_USR0
	MOV r3, GPIO1 | GPIO_CLEARDATAOUT
	SBBO r2, r3, 0, 4
.endm


	MOV r1, 10 			//r1 = 10 om 10x te loop-en
GA:
	USR3_ON			
        MOV r0, DELAYWAARDE		//r0 = delaywaarde
DELAY1:
        SUB r0, r0, 1			//r0 = r0 - 1
        QBNE DELAY1, r0, 0		//jump naar delay wanneer r0 = geen 0
	USR2_ON
	USR3_OFF
        MOV r0, DELAYWAARDE		//weer delay
DELAY2:
        SUB r0, r0, 1
        QBNE DELAY2, r0, 0
        USR1_ON
	USR2_OFF
        MOV r0, DELAYWAARDE		//weer delay
DELAY3:
        SUB r0, r0, 1
        QBNE DELAY3, r0, 0
        USR0_ON
	USR1_OFF
        MOV r0, DELAYWAARDE		//weer delay
DELAY4:
        SUB r0, r0, 1
        QBNE DELAY4, r0, 0
	USR0_OFF
	SUB r1, r1, 1			// r1 = r1 - 1
	QBNE GA, r1, 0			// jump naar blink wanneer r1 = niet 0

EXIT:
    // Send notification to Host for program completion
    //	MOV       r31.b0, PRU1_ARM_INTERRUPT+16

    // Halt the processor
    	HALT			
