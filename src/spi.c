/*
* spi.c - Beagle Bone Black
*	ss pin (out) = P9_28 0x99c gpio113
*	displ power down (out) = P9_23 GPIO1_17 gpio49 0x844
*	display interrupt (in) = P9_12 GPIO1_28 gpio60 0x878
*
*/

//#include <stdint.h>
//#include <unistd.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <getopt.h>
//#include <fcntl.h>
//#include <sys/ioctl.h>
//#include <linux/types.h>
//#include <linux/spi/spidev.h>

#include "FT_GPU.h"
#include "devices.h"
#include "spi.h"

//static const char *device = "/dev/spidev1.0"
//static uint8_t mode = SPI_MODE_0;

/**
 *  \brief Brief
 *  
 *  \param [in] fd Parameter_Description
 *  \param [in] cmd Parameter_Description
 *  \return Return_Description
 *  
 *  \details Details
 */
void command_write(int fd, uint8_t cmd )
{
    printf("Writing CMD :%#02x \n", cmd);
    const uint8_t cmds[] = {cmd, 0x00, 0x00};

    struct spi_ioc_transfer commandos =
    {
        .tx_buf = (unsigned long)cmds,
        .rx_buf = 0, /* null receive data */
        .len = sizeof(cmds),
        .delay_usecs = SPI_DELAY,
        .speed_hz = SPI_SPEED,
        .bits_per_word = WORD_LENGHT,
        .cs_change = 0,
    };
    
    if(ioctl(fd, SPI_IOC_MESSAGE(1), &commandos) < 1)
    {
    	pabort("can't send spi message");
    }
}
 
/**
 *  \brief Writes 32 bits of data to a specified adress by SPI.
 *  
 *  \param [in] fd Filepointer, should be the SPI Device one.
 *  \param [in] Addr, 22 bit address where data will be written to
 *  \param [in] Data, 4 bytes of data to write to the address of Addr
 *  \return Return_Description
 *  
 *  \details Details
 */ 
void mem_wr32(int fd, uint32_t Addr, uint32_t Data)
{
	static uint8_t mask = 0xFF;
    static uint8_t partmask = 0x3F;
    static uint8_t com_mode = 0x80; //voor mem_write

    uint8_t addrMSB, addrMID, addrLSB, datapartMSB, datapartMID1, datapartMID2, datapartLSB;
    
    addrLSB = Addr & mask;						/*!< Detailed description after the member */
    addrMID = (Addr >> 8)&mask; 				/*!< Detailed description after the member */
    addrMSB = ((Addr >> 16)&partmask)|com_mode;	/*!< Detailed description after the member */

    datapartLSB  =  Data & mask;
    datapartMID1 = (Data >> 8) & mask;
    datapartMID2 = (Data >> 16) & mask;
    datapartMSB  = (Data >> 24) & mask;
    
#ifdef DATAFORMAT_LITTLE_ENDIAN
    const uint8_t memwrs32[] = {addrMSB, addrMID, addrLSB, datapartLSB, datapartMID1,  datapartMID2, datapartMSB};
#endif //DATAFORMAT_LITTLE_ENDIAN
#ifdef DATAFORMAT_BIG_ENDIAN
    const uint8_t memwrs32[] = {addrMSB, addrMID, addrLSB, datapartMSB, datapartMID2,  datapartMID1, datapartLSB};
#endif //DATAFORMAT_BIG_ENDIAN



    struct spi_ioc_transfer memwrite32 =
    {
        .tx_buf = (unsigned long)memwrs32,
        .rx_buf = 0, // null receive data
        .len = sizeof(memwrs32),
        .delay_usecs = SPI_DELAY,
        .speed_hz = SPI_SPEED,
        .bits_per_word = WORD_LENGHT,
        .cs_change = 0,
    };
    
    if(ioctl(fd, SPI_IOC_MESSAGE(1), &memwrite32) < 1)
    {
    	pabort("can't send spi message");
    }
}



void mem_wr_str(int fd, uint32_t Addr, uint32_t length, char *s)
{
	static uint8_t mask = 0xFF;
	static uint8_t partmask = 0x3F;
	static uint8_t com_mode = 0x80; //voor mem_write

	uint8_t temp1 = 0, temp2, temp3, temp4;

   uint8_t array[4+length];
   array[0] = ((Addr >> 16)&partmask)|com_mode;
   array[1] = (Addr >> 8)&mask;
   array[2] = Addr & mask;

   strcat(array, s);
while (array[temp1] != 0)
{
	temp1++;
}


#ifdef DATAFORMAT_LITTLE_ENDIAN
   //const uint8_t memwrs[3];//const uint8_t memwrs32[] = {addrMSB, addrMID, addrLSB, datapartLSB, datapartMID1,  datapartMID2, datapartMSB};
   //memwrs[0] = array[0];
   //memwrs[1] = array[1];
   //memwrs[2] = array[2];
   //for(temp2 = 3, temp3 = temp1; temp2 = temp1, temp3 = 3; temp2++, temp3--)
   //{
	//   memwrs[temp2] = array[temp3];
   //}
const uint8_t memwrs[4];
   #endif //DATAFORMAT_LITTLE_ENDIAN
#ifdef DATAFORMAT_BIG_ENDIAN
   const uint8_t memwrs[]
   strcpy(memwrs, array);
#endif //DATAFORMAT_BIG_ENDIAN



   struct spi_ioc_transfer memwrite32 =
   {
       .tx_buf = (unsigned long)memwrs,
       .rx_buf = 0, // null receive data
       .len = sizeof(memwrs),
       .delay_usecs = SPI_DELAY,
       .speed_hz = SPI_SPEED,
       .bits_per_word = WORD_LENGHT,
       .cs_change = 0,
   };

   if(ioctl(fd, SPI_IOC_MESSAGE(1), &memwrite32) < 1)
   {
   	pabort("can't send spi message");
   }
}





















/**
 *  \brief Function which writes specified data bytes with the size of two bytes to a specified address over the SPI bus.
 *  
 *  \param [in] fd, File pointer of the writing operation, in this case it should be the the SPI device pointer.
 *  \param [in] Addr, A 22 bits address where data should be written to.
 *  \param [in] Data, Data with the size of two bytes should be inserted here.
 *  
 *  \details A function which is used to write fout bytes of data to an 22 bits long address over a spi device.
 */
void mem_wr16(int fd, uint32_t Addr, uint16_t Data)
{
	static uint8_t mask = 0xFF;		/*!< A mask which is used to tear the 22 bits adress into single bytes. */
    static uint8_t partmask = 0x3F; /*!< A partial mask which is used to add the com_mode to the address */
    static uint8_t com_mode = 0x80; /*!< com_mode houses the specific 2 MSB (10) for the memory_write function for the FT800 */ 
    uint8_t addrMSB, addrMID, addrLSB, datapartLSB, datapartMSB ;

    addrLSB = Addr & mask;						/*!< AddrLSB is the least significant part of the inserted Address */
    addrMID = (Addr >> 8)&mask; 				/*!< AddrMID is the middle part of the inserted Address */
    addrMSB = ((Addr >> 16)&partmask)|com_mode;	/*!< AddrMSB is the most significant part of the inserted Address */

	/*! @brief: Because the data consists out of two bytes it must be broking into single bytes.
	 * 			For the FT800 data has to be sent with a little endian format and MSB values.
	 *			This means the data has to be broken into multiple bytes with a standard formatting
	 */
	
    datapartLSB  =  Data & mask;
    datapartMSB = (Data >> 8) & mask;

#ifdef DATAFORMAT_LITTLE_ENDIAN
    const uint8_t memwrs16[] = {addrMSB, addrMID, addrLSB, datapartLSB, datapartMSB};
#endif

#ifdef DATAFORMAT_BIG_ENDIAN
    const uint8_t memwrs16[] = {addrMSB, addrMID, addrLSB, datapartMSB, datapartLSB};
#endif


    struct spi_ioc_transfer memwrite16 =
    {
        .tx_buf = (unsigned long)memwrs16,
        .rx_buf = 0, // null receive data
        .len = sizeof(memwrs16),
        .delay_usecs = SPI_DELAY,
        .speed_hz = SPI_SPEED,
        .bits_per_word = WORD_LENGHT,
        .cs_change = 0,
    };

    if(ioctl(fd, SPI_IOC_MESSAGE(1), &memwrite16) < 1)
    {
    	pabort("can't send spi message");
    }
}

/**
 *  \brief Function which writes a specified data byte to a specified address over the SPI bus.
 *  
 *  \param [in] fd, File pointer of the writing operation, in this case it should be the the SPI device pointer.
 *  \param [in] Addr, A 22 bits address where data should be written to.
 *  \param [in] Data, Data with the size of one byte should be inserted here.
 *  
 *  \details A function which is used to write one single byte of data to an 22 bits long address over a spi device.
 */
void mem_wr8(int fd, uint32_t Addr, uint8_t Data)
{
	static uint8_t mask = 0xFF;		/*!< A mask which is used to tear the 22 bits adress into single bytes. */
    static uint8_t partmask = 0x3F; /*!< A partial mask which is used to add the com_mode to the address */
    static uint8_t com_mode = 0x80; /*!< com_mode houses the specific 2 MSB (10) for the memory_write function for the FT800 */ 
    uint8_t addrMSB, addrMID, addrLSB;

    addrLSB = Addr & mask;						/*!< AddrLSB is the least significant part of the inserted Address */
    addrMID = (Addr >> 8)&mask; 				/*!< AddrMID is the middle part of the inserted Address */
    addrMSB = ((Addr >> 16)&partmask)|com_mode;	/*!< AddrMSB is the most significant part of the inserted Address */

    const uint8_t memwrs8[] = {addrMSB, addrMID, addrLSB, Data};

    struct spi_ioc_transfer memwrite8 =
    {
        .tx_buf = (unsigned long)memwrs8,
        .rx_buf = 0, // null receive data
        .len = sizeof(memwrs8),
        .delay_usecs = SPI_DELAY,
        .speed_hz = SPI_SPEED,
        .bits_per_word = WORD_LENGHT,
        .cs_change = 0,
    };

    if(ioctl(fd, SPI_IOC_MESSAGE(1), &memwrite8) < 1)
    {
    	pabort("can't send spi message");
    }
}

uint32_t mem_rd32(int fd, uint32_t Addr)
{
	static uint8_t mask = 0xFF;
    static uint8_t partmask = 0x3F;
    static uint8_t com_mode = 0x00; //voor mem_read
    uint8_t addrMSB, addrMID, addrLSB;
    
    addrLSB = Addr & mask;
    addrMID = (Addr >> 8)&mask;
    addrMSB = ((Addr >> 16)&partmask)|com_mode;
    
    const uint8_t memrds[] = {addrMSB, addrMID, addrLSB, ZERO};
    const uint8_t readback[4];
    
    struct spi_ioc_transfer memread[] =
    {
    		{
    				.tx_buf = (unsigned long)memrds,
    				.rx_buf = 0, // null receive data
    				.len = ARRAY_SIZE(memrds),
    		        .delay_usecs = SPI_DELAY,
    		        .speed_hz = SPI_SPEED,
    		        .bits_per_word = WORD_LENGHT,
    		        .cs_change = 0,
    		},
    		{
    				//.tx_buf = 0,
    				.rx_buf = (unsigned long)readback, // null receive data
    				.len = sizeof readback, //ARRAY_SIZE(readback),
    		        .delay_usecs = SPI_DELAY,
    		        .speed_hz = SPI_SPEED,
    		        .bits_per_word = WORD_LENGHT,
    		        .cs_change = 0,
    		}
    };

    if(ioctl(fd, SPI_IOC_MESSAGE(2), &memread) <1)
    {
    	pabort("can't send spi message");
    }

#ifdef DATAFORMAT_LITTLE_ENDIAN
    return ( ( ( (readback[3]<<24) | (readback[2]<<16) ) | (readback[1]<<8) ) |(readback[0]) );
#endif
#ifdef DATAFORMAT_BIG_ENDIAN
    return ( ( ( (readback[0]<<24) | (readback[1]<<16) ) | (readback[2]<<8) ) |(readback[3]) );
#endif
}

uint32_t mem_rd16(int fd, uint32_t Addr)
{
	static uint8_t mask = 0xFF;
    static uint8_t partmask = 0x3F;
    static uint8_t com_mode = 0x00; //voor mem_read
    uint8_t addrMSB, addrMID, addrLSB;
    
    addrLSB = Addr & mask;
    addrMID = (Addr >> 8)&mask;
    addrMSB = ((Addr >> 16)&partmask)|com_mode;

    const uint8_t memrds[] = {addrMSB, addrMID, addrLSB, 0x00};
    const uint8_t readback[2];

    struct spi_ioc_transfer memread[] =
    {
    		{
    				.tx_buf = (unsigned long)memrds,
    				.rx_buf = 0, // null receive data
    				.len = ARRAY_SIZE(memrds),
    		        .delay_usecs = SPI_DELAY,
    		        .speed_hz = SPI_SPEED,
    		        .bits_per_word = WORD_LENGHT,
    		        .cs_change = 0,
    		},
    		{
    				//.tx_buf = 0,
    				.rx_buf = (unsigned long)readback, // null receive data
    				.len = sizeof readback, //ARRAY_SIZE(readback),
    		        .delay_usecs = SPI_DELAY,
    		        .speed_hz = SPI_SPEED,
    		        .bits_per_word = WORD_LENGHT,
    		        .cs_change = 0,
    		}
    };

    if(ioctl(fd, SPI_IOC_MESSAGE(2), &memread) <1)
    {
    	pabort("can't send spi message");
    }


#ifdef DATAFORMAT_LITTLE_ENDIAN
    return ( (readback[1]<<8)  |(readback[0]) );
#endif
#ifdef DATAFORMAT_BIG_ENDIAN
    return ( (readback[0]<<8)  |(readback[1]) );
#endif
}

uint32_t mem_rd8(int fd, uint32_t Addr)
{
    //int ret;
	static uint8_t mask = 0xFF;
    static uint8_t partmask = 0x3F;
    static uint8_t com_mode = 0x00; //voor mem_read
    uint8_t addrMSB, addrMID, addrLSB;
    uint32_t tmp1 = 0;

    addrLSB = Addr & mask;
    addrMID = (Addr >> 8)&mask;
    addrMSB = ((Addr >> 16)&partmask)|com_mode;

    const uint8_t memrds[] = {addrMSB, addrMID, addrLSB, ZERO, ZERO};
    const uint8_t readback[1] = {ZERO, };

    struct spi_ioc_transfer memread[] =
    {
    		{
    				.tx_buf = (unsigned long)memrds,
    				.rx_buf = 0, // null receive data
    				.len = ARRAY_SIZE(memrds),
    		        .delay_usecs = SPI_DELAY,
    		        .speed_hz = SPI_SPEED,
    		        .bits_per_word = WORD_LENGHT,
    		        .cs_change = 0,
    		},
    		{
    				//.tx_buf = 0,
    				.rx_buf = (unsigned long)readback, // null receive data
    				.len = sizeof readback,//ARRAY_SIZE(readback),
    		        .delay_usecs = SPI_DELAY,
    		        .speed_hz = SPI_SPEED,
    		        .bits_per_word = WORD_LENGHT,
    		        .cs_change = 0,
    		}
    };

    if(ioctl(fd, SPI_IOC_MESSAGE(2), &memread) <1)
    {
    	pabort("can't send spi message");
    }

    return readback[0];
}

/*
 * Open SPI, returns filepointer
 */
int open_SPI(uint8_t spi_mode, uint8_t word_size, uint32_t spi_speed)
{
	int fd;
    if((fd = open("/dev/spidev1.0", O_RDWR))<0)
    {
    	printf("Cant open /dev/spidev1.0");
    	pabort("Can't open device");
    }

    if((ioctl(fd, SPI_IOC_WR_MODE, &spi_mode)) == -1)
    {
       	printf("SPI_INIT:: can't set SPI Mode\n");
      	return 0;
    }

    if((ioctl(fd, SPI_IOC_RD_MODE, &spi_mode)) == -1)
    {
       	printf("SPI_INIT:: can't set SPI Mode\n");
       	return 0;
    }

    if((ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &word_size)) == -1)
    {
       	printf("SPI_INIT:: can't set bits per word\n");
       	return 0;
    }

    if((ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &word_size)) == -1)
    {
       	printf("SPI_INIT:: can't set bits per word\n");
       	return 0;
    }

    if(ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed) == -1)
    {
       	printf("SPI_INIT:: can't set max speed\n");
       	return 0;
    }

    if(ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed) == -1)
    {
       	printf("SPI_INIT:: can't set max speed\n");
       	return 0;
    }

    printf("SPI_INIT:: spi initialised in mode: %d\n", spi_mode);
    printf("SPI_INIT:: spi uses %d bits per word\n", word_size);
    printf("SPI_INIT:: spi clock running at:%d KHz\n", spi_speed/1000);

    return fd;
}

/*
 * Closes spi with filepointer, returns true when finished
 */
bool close_SPI(int fd)
{
    close(fd);
    return TRUE;
}
