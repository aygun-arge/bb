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

// Driver header file
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

/******************************************************************************
* Explicit External Declarations                                              *
******************************************************************************/

/******************************************************************************
* Local Macro Declarations                                                    *
******************************************************************************/

#define PRU_NUM 	 0
#define ADDEND1	 	 0x98765400u
#define ADDEND2		 0x12345678u
#define ADDEND3		 0x10210210u

#define DDR_BASEADDR     0x80000000
#define OFFSET_DDR	 0x00001000
#define OFFSET_SHAREDRAM 2048		//equivalent with 0x00002000

#define PRUSS0_SHARED_DATARAM    4

#define PATH "./loper.bin"

/******************************************************************************
* Local Typedef Declarations                                                  *
******************************************************************************/


/******************************************************************************
* Local Function Declarations                                                 *
******************************************************************************/
static int PRU_setup()

static int LOCAL_exampleInit ( );
static unsigned short LOCAL_examplePassed ( unsigned short pruNum );

/******************************************************************************
* Local Variable Definitions                                                  *
******************************************************************************/


/******************************************************************************
* Intertupt Service Routines                                                  *
******************************************************************************/


/******************************************************************************
* Global Variable Definitions                                                 *
******************************************************************************/

static int mem_fd;
static void *ddrMem, *sharedMem;

static unsigned int *sharedMem_int;

/******************************************************************************
* Global Function Definitions                                                 *
******************************************************************************/

int main (void)
{
	if((PRU_setup()) < 0)
	{
		printf("kan niet initialiseren");
	}		

	/* Initialize example */
	printf("\tINFO: Loading file.\r\n");
	    
		/* load and run the PRU program */
   		if(prussdrv_exec_program (PRU_NUM, PATH) < 0) //verander looper in juiste filenaam
		{
			printf(stderr, "prussdrv_exec_program() failed\n");
		}
		
		printf("\tINFO: Executing programm.\r\n");



    /* Wait until PRU has finished execution */
    printf("\tINFO: Waiting for HALT command.\r\n");
    prussdrv_pru_wait_event (PRU_EVTOUT_0);
    printf("\tINFO: PRU completed transfer.\r\n");
    prussdrv_pru_clear_event (PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);



    /* Disable PRU and close memory mapping*/
    prussdrv_pru_disable(PRU_NUM);
    prussdrv_exit ();

    return(0);
}

/*****************************************************************************
* Local Function Definitions                                                 *
*****************************************************************************/
static int PRU_setup(void)
{
	printf("begin met setup pru\n");
	unsigned int ret; //return var
	//int chk; //check variable
	tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
	
   	/* initialize PRU */
  	 if((ret = prussdrv_init()) != 0)
	{
		printf(stderr, "prussdrv_init() failed\n");
		return ret;
	}

   	/* open PRU interrupt */
   	if((ret = prussdrv_open(PRU_EVTOUT_0)) != 0) 
	{
		printf(stderr, "prussdrv_open() failed\n");
      		return ret;
	}

	/* initialize interrupt */
	if((ret = prussdrv_pruintc_init(&pruss_intc_initdata)) != 0)
	{
		printf(stderr, "prussdrv_pruintc_init() failed\n");
		return ret;
	}
}


/*** pru_cleanup() -- halt PRU and release driver

Performs all necessary de-initialization tasks for the prussdrv library.

Returns 0 on success, non-0 on error.
***/
static int pru_cleanup(void)
{
	int ret = 0;

	/* clear the event (if asserted) */
	if(prussdrv_pru_clear_event(PRU_EVTOUT_0, PRU0_ARM_INTERRUPT))
	{
		fprintf(stderr, "prussdrv_pru_clear_event() failed\n");
		ret = -1;
	}

	/* halt and disable the PRU (if running) */
	if((ret = prussdrv_pru_disable(PRU_NUM)) != 0) 
	{
		fprintf(stderr, "prussdrv_pru_disable() failed\n");
		ret = -1;
	}

   	/* release the PRU clocks and disable prussdrv module */
   	if((ret = prussdrv_exit()) != 0)
	{
		printf(stderr, "prussdrv_exit() failed\n");
		ret = -1;
	}

	return ret;
}

