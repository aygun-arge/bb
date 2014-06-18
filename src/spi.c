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


#include "devices.h"
#include "spi.h"

//static const char *device = "/dev/spidev1.0"
//static uint8_t mode = SPI_MODE_0;

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
    
void memory_write32(int fd, uint32_t Addr, uint32_t Data)
{
	static uint8_t mask = 0xFF;
    static uint8_t partmask = 0x3F;
    static uint8_t com_mode = 0x80; //voor mem_write

    uint8_t addrMSB, addrMID, addrLSB, datapartMSB, datapartMID1, datapartMID2, datapartLSB;
    
    addrLSB = Addr & mask;
    addrMID = (Addr >> 8)&mask;
    addrMSB = ((Addr >> 16)&partmask)|com_mode;
    
    datapartLSB  =  Data & mask;
    datapartMID1 = (Data >> 8) & mask;
    datapartMID2 = (Data >> 16) & mask;
    datapartMSB  = (Data >> 24) & mask;
    
    const uint8_t memwrs32[] = {addrMSB, addrMID, addrLSB, datapartLSB, datapartMID1,  datapartMID2, datapartMSB};
    

    //const uint8_t memwrs32[] = {(((Addr >> 16)&partmask)|com_mode), ((Addr >> 8)&mask), (Addr & mask), (Data & mask), ((Data >> 8) & mask), ((Data >> 16) & mask), ((Data >> 24) & mask)};

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

void memory_write16(int fd, uint32_t Addr, uint16_t Data)
{
    int ret;
	static uint8_t mask = 0xFF;
    static uint8_t partmask = 0x3F;
    static uint8_t com_mode = 0x80; //voor mem_write
    uint8_t addrMSB, addrMID, addrLSB, datapartLSB, datapartMSB ;

    addrLSB = Addr & mask;
    addrMID = (Addr >> 8)&mask;
    addrMSB = ((Addr >> 16)&partmask)|com_mode;

    datapartLSB  =  Data & mask;
    datapartMSB = (Data >> 8) & mask;

    const uint8_t memwrs16[] = {addrMSB, addrMID, addrLSB, datapartLSB, datapartMSB};

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

void memory_write8(int fd, uint32_t Addr, uint8_t Data)
{
    int ret;
	static uint8_t mask = 0xFF;
    static uint8_t partmask = 0x3F;
    static uint8_t com_mode = 0x80; //voor mem_write
    uint8_t addrMSB, addrMID, addrLSB;

    addrLSB = Addr & mask;
    addrMID = (Addr >> 8)&mask;
    addrMSB = ((Addr >> 16)&partmask)|com_mode;

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

uint32_t memory_read32(int fd, uint32_t Addr)
{
    //int ret;
	static uint8_t mask = 0xFF;
    static uint8_t partmask = 0x3F;
    static uint8_t com_mode = 0x00; //voor mem_read
    uint8_t addrMSB, addrMID, addrLSB;
    uint32_t tmp1; //, tmp2;
    
    addrLSB = Addr & mask;
    addrMID = (Addr >> 8)&mask;
    addrMSB = ((Addr >> 16)&partmask)|com_mode;
    
    const uint8_t memrds[] = {addrMSB, addrMID, addrLSB, 0x00};
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
    				.tx_buf = 0,
    				.rx_buf = (unsigned long)readback, // null receive data
    				.len = ARRAY_SIZE(readback),
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

    tmp1 = ( ( ( (readback[3]<<24) | (readback[2]<<16) ) | (readback[1]<<8) ) |(readback[0]) );
    return tmp1;
}

uint32_t memory_read16(int fd, uint32_t Addr)
{
    //int ret;
	static uint8_t mask = 0xFF;
    static uint8_t partmask = 0x3F;
    static uint8_t com_mode = 0x00; //voor mem_read
    uint8_t addrMSB, addrMID, addrLSB;
    uint32_t tmp1;
    
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
    				.tx_buf = 0,
    				.rx_buf = (unsigned long)readback, // null receive data
    				.len = ARRAY_SIZE(readback),
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

    tmp1 = ( (readback[1]<<8)  |(readback[0]) );
    return tmp1;
}

uint32_t memory_read8(int fd, uint32_t Addr)
{
    //int ret;
	static uint8_t mask = 0xFF;
    static uint8_t partmask = 0x3F;
    static uint8_t com_mode = 0x00; //voor mem_read
    uint8_t addrMSB, addrMID, addrLSB;
    uint32_t tmp1;

    addrLSB = Addr & mask;
    addrMID = (Addr >> 8)&mask;
    addrMSB = ((Addr >> 16)&partmask)|com_mode;

    const uint8_t memrds[] = {addrMSB, addrMID, addrLSB, 0x00};
    const uint8_t readback[1];

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
    				.tx_buf = 0,
    				.rx_buf = (unsigned long)readback, // null receive data
    				.len = ARRAY_SIZE(readback),
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

    tmp1 = (readback[0]);
    return tmp1;
}


 int SetupSPI(int fd, uint8_t mode, uint8_t bits, uint32_t speed)
{
    if((ioctl(fd, SPI_IOC_WR_MODE, &mode)) == -1)
    {
    	printf("SPI_INIT:: can't set SPI Mode\n");
    	return 0;
    }

    if((ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits)) == -1)
    {
    	printf("SPI_INIT:: can't set bits per word\n");
    	return 0;
    }

    if(ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1)
    {
    	printf("SPI_INIT:: can't set max speed\n");
    	return 0;
    }

    printf("SPI_INIT:: spi initialised in mode: %d\n", mode);
    printf("SPI_INIT:: spi uses %d bits per word\n", bits);
    printf("SPI_INIT:: spi clock running at:%d KHz\n", speed/1000);

    return 1;
}
