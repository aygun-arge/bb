.origin 0
.entrypoint START

#define GPIO1 0x4804c000
#define GPIO_CLEARDATAOUT 0x190
#define GPIO_SETDATAOUT 0x194


	MOV r1, 10 //r1 = 10
BLINK:
	MOV r2, 7<<22			//r2 = 0b111(21)(20)enz  //USR leds zijn GPIO 1_21 tm _23
        MOV r3, GPIO1 | GPIO_SETDATAOUT // maakt r3 GPIO1 bitwise or setdata = 4804c194
        SBBO r2, r3, 0, 4		//copy 4 bytes van r2 naar r3 offset 0
        MOV r0, 0x00a00000		//r0 = groot getal
DELAY:
        SUB r0, r0, 1			//r0 = r0 - 1
        QBNE DELAY, r0, 0		//jump naar delay wanneer r0 = geen 0
        MOV r2, 7<<22
        MOV r3, GPIO1 | GPIO_CLEARDATAOUT
        SBBO r2, r3, 0, 4
        MOV r0, 0x00a00000		//weer delay
DELAY2:
        SUB r0, r0, 1
        QBNE DELAY2, r0, 0
        SUB r1, r1, 1			// r1 = r1 - 1
	QBNE BLINK, r1, 0		// jump naar blink wanneer r1 = niet 0
					//wanneer r1 = 0, programma dus afgelopen.
