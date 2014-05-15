/* * mytest.c * * Copyright (C) 2013 ADLab - 
http://www.analogdigitallab.org/ 

* * * Redistribution and use in source 
and binary forms, with or without * modification, are permitted provided 
that the following conditions * are met: * * Redistributions of source 
code must retain the above copyright * notice, this list of conditions 
and the following disclaimer. * * Redistributions in binary form must 
reproduce the above copyright * notice, this list of conditions and the 
following disclaimer in the * documentation and/or other materials 
provided with the * distribution. * * Neither the name of Texas 
Instruments Incorporated nor the names of * its contributors may be used 
to endorse or promote products derived * from this software without 
specific prior written permission. * * THIS SOFTWARE IS PROVIDED BY THE 
COPYRIGHT HOLDERS AND CONTRIBUTORS * "AS IS" AND ANY EXPRESS OR IMPLIED 
WARRANTIES, INCLUDING, BUT NOT * LIMITED TO, THE IMPLIED WARRANTIES OF 
MERCHANTABILITY AND FITNESS FOR * A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT * OWNER OR CONTRIBUTORS BE LIABLE FOR 
ANY DIRECT, INDIRECT, INCIDENTAL, * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT * LIMITED TO, PROCUREMENT OF SUBSTITUTE 
GOODS OR SERVICES; LOSS OF USE, * DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY * THEORY OF LIABILITY, WHETHER 
IN CONTRACT, STRICT LIABILITY, OR TORT * (INCLUDING NEGLIGENCE OR 
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE * OF THIS SOFTWARE, EVEN IF 
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. *
*/



/****************************************************************************** 
* Include Files * 
******************************************************************************/ 
// Standard header files
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/mman.h> 
#include <fcntl.h> 
#include <errno.h> 
#include <unistd.h> 
#include <string.h> 
#include <math.h>
// Driver header file
#include <prussdrv.h>
#include <pruss_intc_mapping.h>	 
#include <math.h>

/****************************************************************************** 
* Local Macro Declarations * 
******************************************************************************/
#define PRU_NUM 	 0     

int main(void){

	unsigned int ret;
	unsigned int i,value;

	tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
	printf("\nINFO: Starting %s example.\r\n", "PWM Demo with PRU");
	// Initialize the PRU 
	prussdrv_init();
	// Open PRU Interrupt 
	ret = prussdrv_open(PRU_EVTOUT_0);
	if (ret)
	{
		printf("prussdrv_open open failed\n");
		return (ret);
	}
	

	// Get the interrupt initialized
	prussdrv_pruintc_init(&pruss_intc_initdata);

	//Execute example on PRU
	printf("\tINFO: Executing example.\r\n");
	prussdrv_exec_program(PRU_NUM, "./pwm.bin");
	
        for (i = 0; i < 100000; i++) {
                usleep(100);
		value=(sin(i*0.01)+1)*120+5;
		prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 0x00000040, &value, sizeof(unsigned int));
		printf("(%d)\n-> ",value);
        }


        // Wait until PRU0 has finished execution
        printf("\tINFO: Waiting for HALT command.\r\n");
	
	prussdrv_pru_wait_event(PRU_EVTOUT_0);
	printf("\tINFO: PRU completed transfer.\r\n");
	prussdrv_pru_clear_event(PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);

	// Disable PRU and close memory mapping
	prussdrv_pru_disable(PRU_NUM);
	prussdrv_exit();
	return(0);
}






