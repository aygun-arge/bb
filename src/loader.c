/*
 * loader.c
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */



/******************************************************************************
* Include Files                                                               *
******************************************************************************/

// Standard header files
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

//#include <i2cfunc.h>
// Driver header file
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include "devices.h"

/******************************************************************************
* Explicit External Declarations                                              *
******************************************************************************/

/******************************************************************************
* Local Macro Declarations                                                    *
******************************************************************************/

#define PATH		"memtest.bin" // ./ Hier de filenaam aanpassen

#define PRU_1		1
#define PRU_0		0

#define PRU_NUM 	PRU_1		// hier instellen welke PRU core gebruikt moet worden
					// N.B. ook de interrupt vectoren corrigeren op de pru core keuze

#define DDR_BASEADDR    0x80000000
#define OFFSET_DDR      0x00001008
#define OFFSET_SHAREDRAM 4
#define PRUSS1_SHARED_DATARAM 4
#define SAMPLES 100

#define TRUE 1
#define FALSE 0

#define ADDEND1 0x98765400u
#define ADDEND2 0x12345678u
#define ADDEND3 0x10210210u

#define DEBUG
/******************************************************************************
* Local Typedef Declarations                                                  *
******************************************************************************/


/******************************************************************************
* Local Function Declarations                                                 *
******************************************************************************/
//int Init_RAM( );
int initializePruss( );
void sample2file(void);
int RAM_init( );
int verify_feedback ( );

/******************************************************************************
* Local Variable Definitions                                                  *
******************************************************************************/


/******************************************************************************
* Intertupt Service Routines                                                  *
******************************************************************************/


/******************************************************************************
* Global Variable Definitions                                                 *
******************************************************************************/
//ti dingen

void *ddrMem, *sharedMem;
unsigned int *sharedMem_int;
void *DDR_paramaddr, *DDR_ackaddr;
int mem_fd;

FILE* save_file;

/******************************************************************************
* Global Function Definitions                                                 *
******************************************************************************/

int main ( )
{

    if(initializePruss() != 0)
    {
         printf("ERR:: PRUSS not initialized\n");
         return -1;
    }

    if(RAM_init() != 0)
    {
        printf("ERR:: MEM not propperly initialized\n");
        return -1;
    }

    printf("Start programma!\n");

    prussdrv_exec_program (PRU_1, PATH); // voer binary uit op pru

        printf("\n na execute \n");

        //wacht op halt commando en clear interrupt
        prussdrv_pru_wait_event (PRU_EVTOUT_1);
        prussdrv_pru_clear_event (PRU_EVTOUT_1, PRU1_ARM_INTERRUPT);
        printf("\n interrupt terug\n");

        if((verify_feedback()) != 0)
        {
        	printf("ERR:: fout tussen PRU en geheugen\n");
        	return -1;
        }

        prussdrv_pru_disable(PRU_1);
        prussdrv_exit();
        munmap(ddrMem, 0x0FFFFFFF);
        close(mem_fd);


#ifndef DEBUG

   // printf("\n Opening samples.txt");
   // save_file = fopen("samples.txt", "w");
   // if( save_file == NULL )
   // {
   //     //#ifdef DEBUG
   //printf("\n\nERR: kan file niet openen!\n");
        //#endif //DEBUG
   // }
   // else
   // {
   //     printf("\nGOOD: File geopend\n");
   // }

    //Execute real functionality/

    printf("\INFO: Setting pointers\n");
    DDR_paramaddr = ddrMem + OFFSET_DDR - 8;
    DDR_ackaddr = ddrMem + OFFSET_DDR - 4;

    printf("\nINFO: Schrijf iets in RAM\n");
    sharedMem_int[OFFSET_SHAREDRAM] = 0; //means no command, not used by pru now


    // Execute binary on PRU
    printf("\nINFO: Executing applicatie.bin on PRU1.\n");
    prussdrv_exec_program (PRU_NUM, PATH);

    //sleep(1);

    //sharedMem_int[OFFSET_SHAREDRAM] = 2; //means perform capture, also not used by pru

    printf("\nINFO: Wegschrijven naar samples.txt bereikt\n");
   // if(sharedMem_int[OFFSET_SHAREDRAM] == 1)    //einde pru code bereikt
   // {
	printf("\nGOOD: In schrijf deel\n");
        sample2file();
   //     sharedMem_int[OFFSET_SHAREDRAM] = 0; // bit weer gereset.
   // }
   // else
   // {
        printf("\nERR: niet in schrijfdeel\n");
   // }


    printf("\n\nklaar?\n\n");

    fclose(save_file);



    // Wait until PRU0 has finished execution //
    printf("\tINFO: Waiting for HALT command.\r\n");
//    prussdrv_pru_wait_event (PRU_EVTOUT_1);
    printf("\tINFO: PRU completed transfer.\r\n");
//    prussdrv_pru_clear_event (PRU_EVTOUT_1, PRU1_ARM_INTERRUPT);



    // Disable PRU and close memory mapping/
    prussdrv_pru_disable(PRU_NUM);
    prussdrv_exit ();

    munmap(ddrMem, 0x0FFFFFFF);
    close(mem_fd);

printf("la");
#endif

printf("klaar");
return 0;
}

/******************************************************************************
 * Local Function Definitions                                                 *
 ******************************************************************************/

int initializePruss( )
{
    static int returnx;

    tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

    printf("INIT_PRUSS:: Start initialisatie pru\n");

    //Initialiseer PRU
    prussdrv_init ();
    printf("INIT_PRUSS:: Initialized\n");

    //Open PRU Interrupt
    if( (returnx = prussdrv_open(PRU_EVTOUT_1)) == 1)
    {
        printf("INIT_PRUSS:: prussdrv_open open failed\n");
        return (returnx);
    }
    printf("INIT_PRUSS: PRU Interrupt Opend\n");

    //Initialiseer interrupt
    prussdrv_pruintc_init(&pruss_intc_initdata);
    returnx = 0;
    printf("INIT_PRUSS:: SUCCES!!\n\n");
    return(returnx);
}


int RAM_init( )
{
	void *DDR_regaddr1, *DDR_regaddr2, *DDR_regaddr3;	//maak aantal pointers zonder dataformaat

	if((mem_fd = open(MEM, O_RDWR)) < 0) //Openening memory in READWRITE mode
	{
		printf("INIT_RAM:: geheugen niet geopend!\n");
		return -1;
	}

	//map het DDR geheugen
	if((ddrMem = mmap(0, 0x0FFFFFFF, PROT_WRITE | PROT_READ, MAP_SHARED, mem_fd, DDR_BASEADDR)) == NULL )
	{
		printf("INIT_RAM:: Mapping geheugen niet gelukt\n");
		return -1;
	}

	//Zet de waarden van ADDEND1 in geheugen.
	DDR_regaddr1 = ddrMem + OFFSET_DDR;
	DDR_regaddr2 = ddrMem + OFFSET_DDR + 0x04;
	DDR_regaddr3 = ddrMem + OFFSET_DDR + 0x08;

	*(unsigned long*) DDR_regaddr1 = ADDEND1;
	*(unsigned long*) DDR_regaddr2 = ADDEND2;
	*(unsigned long*) DDR_regaddr3 = ADDEND3;

	printf("INIT_RAM:: mapping voltooid, data geschreven\n");

	return 0;
}

int verify_feedback ( )
{
	printf("in veri\n");
	unsigned int return1, return2, return3;

	prussdrv_map_prumem(PRUSS1_SHARED_DATARAM, &sharedMem);
	printf("in veri2\n");
	sharedMem_int = (unsigned int*) sharedMem;

	return1 = sharedMem_int[OFFSET_SHAREDRAM];
	return2 = sharedMem_int[OFFSET_SHAREDRAM + 1];
	return3 = sharedMem_int[OFFSET_SHAREDRAM + 2];
	printf("in veri3\n");
	if( (return1 == ADDEND1) & (return2 == ADDEND2) &(return3 == ADDEND3) )
	{
		printf("VERIF:: geheugen is correct terug gelezen %d %d %d\n", return1, return2, return3);
		return 0;
	}
	else
	{
		printf("VERIF:: geheugen is niet correct terug gelezen");
		return -1;
	}

}


/*
int Init_RAM( )
{
    prussdrv_map_prumem(PRUSS1_SHARED_DATARAM, &sharedMem);
    sharedMem_int = (unsigned int*) sharedMem;

    printf("mem stage 1\n");

    // open the device
    mem_fd = open("/dev/mem", O_RDWR);
    if (mem_fd < 0) {
        printf("Failed to open /dev/mem (%s)\n", strerror(errno));
        return -1;
    }

    printf("mem stage 2\n");

    //map the DDR memory
    ddrMem = mmap(0, 0x0FFFFFFF, PROT_WRITE | PROT_READ, MAP_SHARED, mem_fd, DDR_BASEADDR);
    if (ddrMem == NULL) {
        printf("Failed to map the device (%s)\n", strerror(errno));
        close(mem_fd);
        return -1;
    }

    printf("mem stage 3\n");
    return(0);
}
*/

/*
void sample2file(void)
{
    int x;
    int *DDR_regaddr;
    int *p_value;
    int value;

    DDR_regaddr = ddrMem + OFFSET_DDR;
    p_value = (unsigned int*)&sharedMem_int[OFFSET_SHAREDRAM+1];

    for (x = 1; x<SAMPLES; x++)
    {
        value = *p_value;
	value = value & 0xfff; //alleen eerste 12 bits
	printf("%d    %d \n", x, value);
        fprintf(save_file," %d\n", value);
        p_value++;
	p_value++;

    }

    printf("\n saved data\n");

}
*/






















