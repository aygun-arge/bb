.origin 0
.entrypoint EXIT

#include "PRU_memAccess_DDR_PRUsharedRAM.hp"

//
// Stop programma om pru ingeval van fout te halten
//


	EXIT:
    // Send notification to Host for program completion
    MOV       r31.b0, PRU1_ARM_INTERRUPT+16

    // Halt the processor
    HALT			