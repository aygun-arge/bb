/*
 * loader.c
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

#define PATH		"./sample.bin" // ./ Hier de filenaam aanpassen
#define FILENAME	"SAMPLES.txt"	// hier filenaam en extentie aanpassen

#define PRU_1		1
#define PRU_0		0

#define PRU_NUM 	PRU_1		// hier instellen welke PRU core gebruikt moet worden
					// N.B. ook de interrupt vectoren corrigeren op de pru core keuze

#define DDR_BASEADDR     0x80000000
#define OFFSET_DDR       0x00001000
#define OFFSET_SHAREDRAM 0x00000000 //x2000 voor RAM1 pru0, RAM0 voor pru 1
#define PRUSS1_SHARED_DATARAM 4
#define SAMPLES 3000

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
int Save_Samples ( );
int PRUSS_RAM_init( );
unsigned int test_match ( );

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
int *Shared_addr;
int mem_fd;
int blabla;
int test;

/******************************************************************************
* Global Function Definitions                                                 *
******************************************************************************/

int handlepru ( )
{
	gpio_export(ADC_BUF);
	gpio_set_dir(ADC_BUF, OUTPUT);
	gpio_set_value(ADC_BUF, HIGH);

int p = 0;
    if(initializePruss() != 0)
    {
         printf("ERR:: PRUSS not initialized\n");
         return -1;
    }

    if(PRUSS_RAM_init() != 0)
    {
        printf("ERR:: MEM not propperly initialized\n");
        return -1;
    }

    printf("Start programma!\n");

    prussdrv_exec_program (PRU_1, PATH); // voer binary uit op pru

    printf("\n na execute \n");

    usleep(1000); //wacht 1 mSec
    if((test_match()) != 0)
    {
       	printf("ERR:: fout tussen PRU en geheugen\n");
        return -1;
    }

    printf("INFO:: Memory matching goed, shared ram gecleared!\n");

    //stuur een ack voor pru op shared mem locatie 3 (vanaf 0)
    sharedMem_int[OFFSET_SHAREDRAM + 3] = 0x01u;

#ifndef DEBUG
    for(test = 0; test < 5; test++)
    {
    	blabla = sharedMem_int[OFFSET_SHAREDRAM + test];
    	printf("\n%d, %#08x\n", test, blabla);
    }
#endif

    while((sharedMem_int[OFFSET_SHAREDRAM]!=0x01)&&(sharedMem_int[OFFSET_SHAREDRAM + 3]!= 0x08))
    {
    	printf("\rno reply pru yet");
    	//stop de pru?
    	//return -1;
    }
    //stuur start sample commando naar pru 0x02 op sharedmem[offset+0]
    printf("INFO:: reply correct\n\n");
    gpio_set_value(ADC_BUF, LOW);
    sharedMem_int[OFFSET_SHAREDRAM + 3] = SAMPLES;
    sharedMem_int[OFFSET_SHAREDRAM] = 0x02;

    //wacht tot pru iets heeft gedaan (1 sec = 200.000.000 instructies, dus pru waarschijnlijk klaar).

    ///while(sharedMem_int[OFFSET_SHAREDRAM]!= 0x03)
    //{
   // 	printf("waiting!!!\r");
    	//usleep(1);
    //}

    printf("\nPRU 1 \n");
    sleep(5);
    printf("\nPRU 2 \n");
    do
    {
        printf("\nPRU 3 \n");
    	if (sharedMem_int[OFFSET_SHAREDRAM] == 0x03)
    	{
    	    printf("\nPRU 4 \n");
    		if(Save_Samples() != 0)
    	    {
    	    	printf("Sample save mislukt\n");
    	    }
    	    sharedMem_int[OFFSET_SHAREDRAM] = 0x04;
    	    p = 1;
    	    printf("\ndone\n");
    	}

    }while (!p);


    //if(Save_Samples() != 0)
    //{
    //	printf("Sample save mislukt\n");
    //}

   // sharedMem_int[OFFSET_SHAREDRAM] = 0x04;

    //wacht op halt commando en clear interrupt
    prussdrv_pru_wait_event (PRU_EVTOUT_1);

    prussdrv_pru_clear_event (PRU_EVTOUT_1, PRU1_ARM_INTERRUPT);
    gpio_set_value(ADC_BUF, HIGH);
    printf("\n interrupt terug\n");

    prussdrv_pru_disable(PRU_1);
    //clear geheugen voor de zekerheid
	sharedMem_int[OFFSET_SHAREDRAM] = 0;
	sharedMem_int[OFFSET_SHAREDRAM + 3] = 0;

    prussdrv_exit();
    munmap(ddrMem, 0x0FFFFFFF);
    close(mem_fd);

	printf("klaar");
	return 0;
}

/******************************************************************************
 * Local Function Definitions                                                 *
 ******************************************************************************/

 /**
 *  \brief Brief
 *  
 *  \return returnx
 *  
 *  \details Details
 */
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

int PRUSS_RAM_init( )
{
	//Open het memory
	if((mem_fd = open(MEM, O_RDWR)) < 0) //Openening memory in READWRITE mode
	{
		printf("INIT_RAM:: geheugen niet geopend!\n");
		return -1;
	}

	//map het DDR geheugen
	if((ddrMem = mmap(0, 0x0FFFFFFF, PROT_WRITE | PROT_READ, MAP_SHARED, mem_fd, DDR_BASEADDR)) == NULL )
	{
		printf("INIT_RAM:: Mapping geheugen niet gelukt\n");
		close(mem_fd);
		return -1;
	}

	//map het Shared MEM van PRU
	prussdrv_map_prumem(PRUSS1_SHARED_DATARAM, &sharedMem);
	sharedMem_int = (unsigned int*) sharedMem;

	DDR_paramaddr = ddrMem + OFFSET_DDR - 8;
	DDR_ackaddr = ddrMem + OFFSET_DDR - 4;

	//Zet inhoud shared geheugen naar 0
	sharedMem_int[OFFSET_SHAREDRAM] = 0;
	sharedMem_int[OFFSET_SHAREDRAM + 1] = 0;
	sharedMem_int[OFFSET_SHAREDRAM + 2] = 0;
	sharedMem_int[OFFSET_SHAREDRAM + 3] = 0;
	sharedMem_int[OFFSET_SHAREDRAM + 4] = 0;

	printf("INIT_RAM:: mapping DDR en Shared voltooid\n");

	return 0;
}

unsigned int test_match ( )
{
	void *DDR_regaddr1, *DDR_regaddr2, *DDR_regaddr3;	//maak aantal pointers zonder dataformaat
	unsigned int return1, return2, return3;

	//Zet de waarden van ADDEND1 in geheugen.
	DDR_regaddr1 = ddrMem + OFFSET_DDR;
	DDR_regaddr2 = ddrMem + OFFSET_DDR + 0x04;
	DDR_regaddr3 = ddrMem + OFFSET_DDR + 0x08;

	*(unsigned long*) DDR_regaddr1 = ADDEND1;
	*(unsigned long*) DDR_regaddr2 = ADDEND2;
	*(unsigned long*) DDR_regaddr3 = ADDEND3;

	return1 = sharedMem_int[OFFSET_SHAREDRAM];
	return2 = sharedMem_int[OFFSET_SHAREDRAM + 1];
	return3 = sharedMem_int[OFFSET_SHAREDRAM + 2];

	if( (return1 == ADDEND1) & (return2 == ADDEND2) &(return3 == ADDEND3) )
	{
		printf("VERIF:: geheugen is correct terug gelezen %#08x %#08x %#08x\n", return1, return2, return3);

		//Zet inhoud shared geheugen terug naar 0
		//sharedMem_int[OFFSET_SHAREDRAM] = 0;
		//sharedMem_int[OFFSET_SHAREDRAM + 1] = 0;
		//sharedMem_int[OFFSET_SHAREDRAM + 2] = 0;
		//sharedMem_int[OFFSET_SHAREDRAM + 3] = 0;
		//sharedMem_int[OFFSET_SHAREDRAM + 4] = 0;
		return 0;
	}
	else
	{
		printf("VERIF:: geheugen is niet correct terug gelezen %#08x %#08x %#08x\n", return1, return2, return3);
		return -1;
	}
}

int Save_Samples ( )
{
    unsigned short int *p_value;//16 bits
    unsigned short int *Register_Address;
    unsigned short int value;	//16 bits
    int x;

    p_value = (unsigned short int*)&sharedMem_int[OFFSET_SHAREDRAM+1];

    FILE* save_file;

    printf("SAVE:: Open : %s in write mode\n", FILENAME);

    if((save_file = fopen(FILENAME, "w")) == NULL)
    {
    	printf("ERR_SAVE:: Kan %s niet openen!\n", FILENAME);
    	return -1;
    }
    printf("SAVE:: File correct geopend, start sampling\n");

    //Neem zoveel samples als aangegeven in macro SAMPLES en AND deze om alleen de 12 bits te krijgen
    printf("This file named: %s contains sample values, use with caution!\n", FILENAME);
    fprintf(save_file,"This file named: %s contains sample values, use with caution!\n", FILENAME);

    printf("nummer,\t int,\t hex \n");
    fprintf(save_file,"nummer,\t int,\t hex \n");

    for (x = 1; x<SAMPLES; x++)
    {
        value = *p_value;

        //van 12b signed naar 16b signed

        //value = (value << 4 ) >> 4;

        //value = value & 0xfff; //alleen eerste 12 bits
        //weergeef samplenummer, de int waarde en de hex waarde.
        //printf("%d,\t%d,\t%#016x, \t %#016x \n", x, value, value, p_value);
        fprintf(save_file,"%d\n", value);
        p_value = p_value + 2;
    }
    close(save_file);
    printf("SAVE:: Data geschreven in %s", FILENAME);

    return 0;

}























