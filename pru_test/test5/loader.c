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
#include "prussdrv.h"
#include <pruss_intc_mapping.h>	 

/******************************************************************************
* Explicit External Declarations                                              *
******************************************************************************/

/******************************************************************************
* Local Macro Declarations                                                    *
******************************************************************************/

#define PATH		"./applicatie.bin" //Hier de filenaam aanpassen

#define PRU_1		1
#define PRU_0		0


#define PRU_NUM 	PRU_1		// hier instellen welke PRU core gebruikt moet worden
					// N.B. ook de interrupt vectoren corrigeren op de pru core keuze

#define DDR_BASEADDR    0x80000000
#define OFFSET_DDR      0x00001008
#define OFFSET_SHAREDRAM 0
#define PRUSS1_SHARED_DATARAM 4
#define SAMPLES 100

/******************************************************************************
* Local Typedef Declarations                                                  *
******************************************************************************/


/******************************************************************************
* Local Function Declarations                                                 *
******************************************************************************/
static int LOCAL_exampleInit ( );
unsigned int initializePruss(void);
void sample2file(void)
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
static int mem_fd;
static void *ddrMem, *sharedMem;
static unsigned int *sharedMem_int;


//andere dingen
static int chunk;


FILE* save_file;

/******************************************************************************
* Global Function Definitions                                                 *
******************************************************************************/

int main (void)
{
    
    int i;
    void *DDR_paramaddr;
    void *DDR_ackaddr;
    int fin;
    char fname_new[255];
    
    
    if(initializePruss() != TRUE)
    {
        return(-1);
    }
    


    /* doe dingen */
    save_file = fopen("samples.txt", "r+");
    
    /* Execute mem example */
    printf("\tINFO: Executing example function.\r\n");
    LOCAL_exampleInit(PRU_NUM);
    
    /*Execute real functionality*/
    
    DDR_paramaddr = ddrMem + OFFSET_DDR - 8;
    DRR_ackaddr = ddrMem + OFFSET_DDR - 4;
    
    sharedMem_int[OFFSET_SHAREDRAM] = 0; //mean no command, not used by pru now
    
    
    /* Execute binary on PRU */
    printf("\tINFO: Executing binary on pru.\r\n");
    prussdrv_exec_program (PRU_NUM, PATH);
    
    //sleep(1);
    
    //sharedMem_int[OFFSET_SHAREDRAM] = 2; //means perform capture, also not used by pru
    
    if(sharedMem_int[OFFSET_SHAREDRAM] == 1)    //einde pru code bereikt
    {
        sample2file();
        sharedMem_int[OFFSET_SHAREDRAM] = 0; // bit weer gereset.
    }
    
    printf("\n\nklaar?\n\n");
    
    fclose(save_file);
    
    
    
    /* Wait until PRU0 has finished execution */
    //printf("\tINFO: Waiting for HALT command.\r\n");
    //prussdrv_pru_wait_event (PRU_EVTOUT_1);
    //printf("\tINFO: PRU completed transfer.\r\n");
    //prussdrv_pru_clear_event (PRU_EVTOUT_1, PRU1_ARM_INTERRUPT);


    
    /* Disable PRU and close memory mapping*/
    prussdrv_pru_disable(PRU_NUM); 
    prussdrv_exit ();
    
    munmap(ddrMem, 0x0FFFFFFF);
    close(mem_fd);

    return(0);
}

/******************************************************************************
 * Local Function Definitions                                                 *
 ******************************************************************************/

unsigned int initializePruss(void)
{
    char var;   //wordt gebruikt om te controleren of alles goed gaat.
    unsigned int ret;
    
    tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
    
    printf("\nINFO: Starting %s \r\n", PATH);
    /* Initialize the PRU */
    prussdrv_init ();
    printf("\nINFO: Initialized\n");
    
    /* Open PRU Interrupt */
    ret = prussdrv_open(PRU_EVTOUT_1);
    if (ret)
    {
        printf("prussdrv_open open failed\n");
        var = FALSE;
        return (var);
    }
    
    printf("\nINFO: PRU Interrupt Opend\n");
    
    var = TRUE;
    /* Get the interrupt initialized */
    prussdrv_pruintc_init(&pruss_intc_initdata);
 
    return(var);
}



static int LOCAL_exampleInit (  )
{
    void *DDR_regaddr1, *DDR_regaddr2, *DDR_regaddr3;
    
    /* open the device */
    mem_fd = open("/dev/mem", O_RDWR);
    if (mem_fd < 0) {
        printf("Failed to open /dev/mem (%s)\n", strerror(errno));
        return -1;
    }
    
    /* map the DDR memory */
    ddrMem = mmap(0, 0x0FFFFFFF, PROT_WRITE | PROT_READ, MAP_SHARED, mem_fd, DDR_BASEADDR);
    if (ddrMem == NULL) {
        printf("Failed to map the device (%s)\n", strerror(errno));
        close(mem_fd);
        return -1;
    }
    
    /* Store Addends in DDR memory location */
    DDR_regaddr1 = ddrMem + OFFSET_DDR;
    DDR_regaddr2 = ddrMem + OFFSET_DDR + 0x00000004;
    DDR_regaddr3 = ddrMem + OFFSET_DDR + 0x00000008;
    
    *(unsigned long*) DDR_regaddr1 = ADDEND1;
    *(unsigned long*) DDR_regaddr2 = ADDEND2;
    *(unsigned long*) DDR_regaddr3 = ADDEND3;
    
    return(0);
}



void sample2file(void)
{
    unsigned short int *DDR_regaddr;
    unsigned short int *p_value;
    unsigned short int value;
    
    DDR_regaddr = ddrMem + OFFSET_DDR;
    p_value = (unsigned short int*)&sharedMem_int[OFFSET_SHAREDRAM+1];
    for (int x = 1; x<SAMPLES; x++)
    {
        value = *p_value;
        fprintf(save_file," %d\n", value);
        p_value = p_value + 2;
        
    }
    
    printf("\n saved data\n");
    
}