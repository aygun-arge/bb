.origin 0
.entrypoint START

#include "pwm.hp"

#define GPIO1 0x4804c000
#define GPIO_CLEARDATAOUT 0x190
#define GPIO_SETDATAOUT 0x194
#define MEM_START 0x00000100

START:
//set ARM such that PRU can write to GPIO
LBCO r0, C4, 4, 4
CLR r0, r0, 4
SBCO r0, C4, 4, 4



CLR r30.t14
SET r30,14

//for(r0=0x0100000;r0>0;ro--){
MOV r0,0x01000000


LOOP1:
	  CLR r30.t14		//clear pin GPIO1_12

          MOV r18, MEM_START
          LBBO r1, r18, 0, 4
          AND r1, r1, 0x000000FF
DELAY2:	  SUB r1, r1 , 1	//if r1=100 =>delay 1us
	  QBNE DELAY2, r1, 0	

          SET r30,14		//set pin GPIO44
          MOV r18, MEM_START
          LBBO r1, r18, 0, 4
	  AND r1, r1, 0x000000FF
	  MOV r2, 0x00000100
	  RSB r1, r1, r2
DELAY3:   SUB r1, r1 , 1
          QBNE DELAY3, r1, 0

	  SUB r0, r0, 1
	  QBNE LOOP1, r0, 0
	 


          MOV r31.b0, PRU0_ARM_INTERRUPT+16  //tell the c program we are done 
//(just remove it if your c program does not handle the interrupt)
          HALT      // Halt the processor
 
