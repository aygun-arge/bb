/*
* spi.c - Beagle Bone Black
*	ss pin (out) = P9_28 0x99c gpio113
*	displ power down (out) = P9_23 GPIO1_17 gpio49 0x844
*	display interrupt (in) = P9_12 GPIO1_28 gpio60 0x878
*
*/

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include "FT_DATATYPES.h"
#include "FT_GPU_HAL.h"
#include "FT_GPU.h"
#include "FT_HAL_UTILS.h"
#include "FT_Platform.h"

#define ADRESSTEST 0x3AAAAA
#define WRITETEST 0xFF8100C3


#include "devices.h"

//typedef uint16_t spidata_t;

#define ARRAY_SIZE(a) ( (sizeof(a)/sizeof(uint16_t)) * 2) /* lens in bytes */

static const char *device = "/dev/spidev1.0";
static uint8_t mode = SPI_MODE_0;
static uint8_t bits = 8;
static uint32_t speed = 6000000;
static uint16_t delay = 0;
static uint8_t one = 1;
static uint8_t zero = 0;

static void pabort(const char *s) {
    perror(s);
    abort();
}



void command_write(int fd, uint8_t cmd )
{
    int ret;
    printf("Writing CMD :%#02x \n", cmd);
    const uint8_t cmds[] = {cmd, FT_ZERO, FT_ZERO};

    struct spi_ioc_transfer commandos =
    {
        .tx_buf = (unsigned long)cmds,
        .rx_buf = 0, /* null receive data */
        .len = sizeof(cmds),
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = 8,
        .cs_change = 0,
    };
    
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &commandos);
    if (ret < 1)
        pabort("can't send spi message");
    //usleep(50);
}
    
void memory_write32(int fd, uint32_t Addr, uint32_t Data)
{
    int ret;
	static uint8_t mask = 0xFF;
    static uint8_t partmask = 0x3F;
    static uint8_t com_mode = 0x80; //voor mem_write
    uint8_t addrMSB, addrMID, addrLSB, datapartMSB, datapartMID1, datapartMID2, datapartLSB;
    //static uint32_t tmp1, tmp2;
    
    addrLSB = Addr & mask;
    addrMID = (Addr >> 8)&mask;
    addrMSB = ((Addr >> 16)&partmask)|com_mode;
    
    datapartLSB  =  Data & mask;
    datapartMID1 = (Data >> 8) & mask;
    datapartMID2 = (Data >> 16) & mask;
    datapartMSB  = (Data >> 24) & mask;
    
    printf("Writing DATA :%#08x to ADRESS:%#08x \n", Data, Addr);

    const uint8_t memwrs32[] = {addrMSB, addrMID, addrLSB, datapartLSB, datapartMID1,  datapartMID2, datapartMSB};
    
    struct spi_ioc_transfer memwrite32 =
    {
        .tx_buf = (unsigned long)memwrs32,
        .rx_buf = 0, // null receive data
        .len = sizeof(memwrs32),
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = 8,
        .cs_change = 0,
    };
    
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &memwrite32);
    if (ret < 1)
        pabort("can't send spi message");
    //usleep(50);
}

void memory_write8(int fd, uint32_t Addr, uint8_t Data)
{
    int ret;
	static uint8_t mask = 0xFF;
    static uint8_t partmask = 0x3F;
    static uint8_t com_mode = 0x80; //voor mem_write
    uint8_t addrMSB, addrMID, addrLSB;
    //static uint32_t tmp1, tmp2;

    addrLSB = Addr & mask;
    addrMID = (Addr >> 8)&mask;
    addrMSB = ((Addr >> 16)&partmask)|com_mode;

    printf("Writing DATA :%#08x to ADRESS:%#08x \n", Data, Addr);

    const uint8_t memwrs8[] = {addrMSB, addrMID, addrLSB, Data};

    struct spi_ioc_transfer memwrite8 =
    {
        .tx_buf = (unsigned long)memwrs8,
        .rx_buf = 0, // null receive data
        .len = sizeof(memwrs8),
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = 8,
        .cs_change = 0,
    };

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &memwrite8);
    if (ret < 1)
        pabort("can't send spi message");
    //usleep(50);
}

void memory_write16(int fd, uint32_t Addr, uint16_t Data)
{
    int ret;
	static uint8_t mask = 0xFF;
    static uint8_t partmask = 0x3F;
    static uint8_t com_mode = 0x80; //voor mem_write
    uint8_t addrMSB, addrMID, addrLSB, datapartLSB, datapartMSB ;
    //static uint32_t tmp1, tmp2;

    addrLSB = Addr & mask;
    addrMID = (Addr >> 8)&mask;
    addrMSB = ((Addr >> 16)&partmask)|com_mode;

    datapartLSB  =  Data & mask;
    datapartMSB = (Data >> 8) & mask;


    printf("Writing DATA :%#08x to ADRESS:%#08x \n", Data, Addr);

    const uint8_t memwrs16[] = {addrMSB, addrMID, addrLSB, datapartLSB, datapartMSB};

    struct spi_ioc_transfer memwrite16 =
    {
        .tx_buf = (unsigned long)memwrs16,
        .rx_buf = 0, // null receive data
        .len = sizeof(memwrs16),
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = 8,
        .cs_change = 0,
    };

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &memwrite16);
    if (ret < 1)
        pabort("can't send spi message");
    //usleep(50);
}



uint32_t memory_read(int fd, uint32_t Addr)
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
    
    const uint8_t memrds[] = {addrMSB, addrMID, addrLSB, FT_ZERO};
    const uint8_t readback[4];
    
    struct spi_ioc_transfer memread[] =
    {
    		{
    				.tx_buf = (unsigned long)memrds,
    				.rx_buf = 0, // null receive data
    				.len = ARRAY_SIZE(memrds),
    				.delay_usecs = delay,
    				.speed_hz = speed,
    				.bits_per_word = 8,
    				.cs_change = 0,
    		},
    		{
    				.tx_buf = 0,
    				.rx_buf = (unsigned long)readback, // null receive data
    				.len = ARRAY_SIZE(readback),
    				.delay_usecs = delay,
    				.speed_hz = speed,
    				.bits_per_word = 8,
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

/*
static void transferData(int fd)
{
    int ret;

    // data, 16 bits,
    static const uint16_t data[] = {0x8000};


    struct spi_ioc_transfer test_spi = {
        .tx_buf = (unsigned long)data,
        //.tx_buf = (unsigned long)data,
        .rx_buf = 0, // null receive data
        .len = ARRAY_SIZE(data),
        //.len = size,
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = bits,
    };

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &test_spi);
    if (ret < 1)
        pabort("can't send spi message");

    //usleep(50);
}

*/

static void print_usage(const char *prog) {
    printf("Usage: %s [-D]\n", prog);
    puts(" -D --device device to use (default /dev/spidev1.0)\n");
    exit(1);
}

static void parse_opts(int argc, char *argv[]) {
    while (1) {
        static const struct option lopts[] = {
            { "device", 1, 0, 'D' },
            { NULL, 0, 0, 0 },
        };
        int c;

        c = getopt_long(argc, argv, "D:", lopts, NULL);

        if (c == -1)
            break;

        switch (c) {
        case 'D':
            device = optarg;
            break;
        default:
            print_usage(argv[0]);
            break;
        }
    }
}

int SetupSPI(int fd, uint8_t mode, uint8_t bits, uint32_t speed)
{
    if((ioctl(fd, SPI_IOC_WR_MODE, &mode)) == -1)
    {
    	printf("SPI_INIT:: can't set SPI Mode\n");
    	return 1;
    }

    if((ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits)) == -1)
    {
    	printf("SPI_INIT:: can't set bits per word\n");
    	return 1;
    }

    if(ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1)
    {
    	printf("SPI_INIT:: can't set max speed\n");
    	return 1;
    }

    printf("spi initialised in mode: %d\n", mode);
    printf("spi uses %d bits per word\n", bits);
    printf("spi clock running at:%d KHz\n", speed/1000);

    return 0;
}


void StartDisplay(int fd)
{
	gpio_set_value(DISPL_PWRDWN, LOW);
    usleep(30000);
    gpio_set_value(DISPL_PWRDWN, HIGH);
    usleep(30000);
	command_write(fd, 0x00);
    //usleep(30000);
    command_write(fd, 0x44);
    command_write(fd, 0x62);
    printf("%#08x \n",memory_read(fd, 0x102400));
}



int main(int argc, char *argv[]) {

	gpio_export(DISPL_PWRDWN);
	gpio_set_dir(DISPL_PWRDWN, OUTPUT);
	gpio_export(INTERRUPT_DISPL);
	gpio_set_dir(INTERRUPT_DISPL, INPUT);

	//gpio_get_value(INTERRUPT_DISPL);

	//int ret = 0;
    int fd;
    //int x;

    parse_opts(argc, argv);


    if((fd = open(SPI1, O_RDWR))<0)
    {
    	pabort("Can't open device");
    }

    if((ioctl(fd, SPI_IOC_WR_MODE, &mode)) == -1)
    {
    	printf("SPI_INIT:: can't set SPI Mode\n");
    	return 1;
    }

    if((ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits)) == -1)
    {
    	printf("SPI_INIT:: can't set bits per word\n");
    	return 1;
    }

    if(ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1)
    {
    	printf("SPI_INIT:: can't set max speed\n");
    	return 1;
    }

/*/
    if((ioctl(fd, SPI_IOC_WR_MODE, &mode)) == -1)
    {
        printf("SPI_INIT:: can't set SPI Mode\n");
        return 1;
    }

    if((ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits)) == -1)
    {
        printf("SPI_INIT:: can't set bits per word\n");
        return 1;
    }

    if(ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1)
    {
       	printf("SPI_INIT:: can't set max speed\n");
       	return 1;
    }
*/





    StartDisplay(fd);
    //instellingen voor AT043B35-15I-10 (WQVGA 480x272)

    memory_write16(fd, REG_HCYCLE , 548);
    memory_write16(fd, REG_HOFFSET , 43);
    memory_write16(fd, REG_HSYNC0 , 0);
    memory_write16(fd, REG_HSYNC1 , 41);

    memory_write16(fd, REG_VCYCLE , 292);
    memory_write16(fd, REG_VOFFSET , 12);
    memory_write16(fd, REG_VSYNC0 , 0);
    memory_write16(fd, REG_VSYNC1 , 10);

    memory_write8(fd, REG_SWIZZLE, 0);
    memory_write8(fd, REG_PCLK_POL, 1);
    memory_write8(fd, REG_CSPREAD, 1);

    memory_write16(fd, REG_VSIZE , 272);
    memory_write16(fd, REG_HSIZE , 480);

//eerste displaylist
    memory_write32(fd, RAM_DL+0, CLEAR_COLOR_RGB(255,140,0));
    memory_write32(fd, RAM_DL+4, CLEAR(1,1,1));
    memory_write32(fd, RAM_DL+8, DISPLAY());

    memory_write8(fd, REG_DLSWAP, DLSWAP_FRAME);

    memory_write8(fd, REG_GPIO_DIR, 0x80);
    memory_write8(fd, REG_GPIO, 0x80);

    memory_write8(fd, REG_PCLK, 5);

while(1)
{
	int d = 0;
}


    close(fd);
	gpio_unexport(DISPL_PWRDWN);
	gpio_unexport(INTERRUPT_DISPL);
    //return ret;
}
