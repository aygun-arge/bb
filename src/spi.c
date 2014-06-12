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

#include "devices.h"

typedef uint16_t spidata_t;

#define ARRAY_SIZE(a) ( (sizeof(a)/sizeof(spidata_t)) * 2) /* lens in bytes */

static const char *device = "/dev/spidev1.0";
static uint8_t mode = SPI_MODE_0;
static uint8_t bits = 8;
static uint32_t speed = 6000000;
static uint16_t delay = 0;

static void pabort(const char *s) {
    perror(s);
    abort();
}


static void StartDisplay(int fd)
{
    int ret;
	gpio_set_value(DISPL_PWRDWN, LOW);
    usleep(20000);
    gpio_set_value(DISPL_PWRDWN, HIGH);

    command_write(FT_GPU_ACTIVE_M);
    command_write(FT_GPU_EXTERNAL_OSC);
    command_write(FT_GPU_PLL_48M);
}

static void command_write(int fd, uint8_t cmd )
{
    static uint8_t com_mode 0x10; //voor command_write
    uint8_t tmp;
    tmp = (cmd & 0x3F) | com_mode;
    const uint8_t cmds[] = {tmp, FT_ZERO, FT_ZERO};

    struct spi_ioc_transfer commandos
    {
        
        .tx_buf = (unsigned char)cmds,
        .rx_buf = 0, /* null receive data */
        .len = ARRAY_SIZE(cmds),
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = 8,
    };
    
    if ((ioctl(fd, SPI_IOC_MESSAGE(1), &commandos)) < 1) pabort("can't send commands");
    usleep(50);
}
    
static void memory_write(int fd, uint32_t Addr, uint32_t Data)
{
    static uint8_t mask = 0xFF;
    static uint8_t partmask = 0x3F;
    static uint8_t com_mode 0x40; //voor mem_write
    uint8_t addrMSB, addrMID, addrLSB, datapartMSB, datapartMID1, datapartMID2, datapartLSB;
    static uint32_t tmp1, tmp2;
    
    addrLSB = Addr & mask;
    addrMID = (Addr >> 8)&mask;
    addrMSB = ((Addr >> 16)&partmask)|com_mode;
    
    datapartLSB  =  Data & mask;
    datapartMID1 = (Data >> 8) & mask;
    datapartMID2 = (Data >> 16) & mask;
    datapartMSB  = (Data >> 24) & mask;
    
    const uint8_t memwrs[] = {addrMSB, addrMID, addrLSB, datapartMSB, datapartMID2, datapartMID1, datapartLSB};
    
    struct spi_ioc_transfer memwrite
    {
        .tx_buf = (unsigned char)memwrs,
        .rx_buf = 0, // null receive data
        .len = ARRAY_SIZE(memwrs),
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = 8,
    };
    
    if ((ioctl(fd, SPI_IOC_MESSAGE(1), &memwrite)) < 1) pabort("can't write to memory");
    usleep(50);
}

static void memory_read(int fd, uint32_t Addr, uint32_t Data)
{
    static uint8_t mask = 0xFF;
    static uint8_t partmask = 0x3F;
    static uint8_t com_mode 0x00; //voor mem_read
    uint8_t addrMSB, addrMID, addrLSB, datapartMSB, datapartMID1, datapartMID2, datapartLSB;
    static uint32_t tmp1, tmp2;
    
    addrLSB = Addr & mask;
    addrMID = (Addr >> 8)&mask;
    addrMSB = ((Addr >> 16)&partmask)|com_mode;
    
    const uint8_t memrds[] = {addrMSB, addrMID, addrLSB};
    
    struct spi_ioc_transfer memwrite
    {
        .tx_buf = (unsigned char)memrds,
        .rx_buf = 0, // null receive data
        .len = ARRAY_SIZE(memrds),
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = 8,
    };
    
    if ((ioctl(fd, SPI_IOC_MESSAGE(1), &memread)) < 1) pabort("can't write to memory");
    usleep(50);
}




/*
static void transferData(int fd, int send_data, int size)
{
    int ret;

    // data, 16 bits,
    static const spidata_t data[] = {0x1111};


    struct spi_ioc_transfer test_spi = {
        .tx_buf = (unsigned long)send_data,
        //.tx_buf = (unsigned long)data,
        .rx_buf = 0, // null receive data
        //.len = ARRAY_SIZE(data),
        .len = size,
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = bits,
    };

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &test_spi);
    if (ret < 1)
        pabort("can't send spi message");

    usleep(50);
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

int main(int argc, char *argv[]) {

	gpio_export(DISPL_PWRDWN);
	gpio_set_dir(DISPL_PWRDWN, OUTPUT);
	gpio_export(INTERRUPT_DISPL);
	gpio_set_dir(INTERRUPT_DISPL, INPUT);

	//gpio_get_value(INTERRUPT_DISPL);



	int ret = 0;
    int fd;
    int size_data;

    parse_opts(argc, argv);

    fd = open(SPI1, O_RDWR);
    if (fd < 0)
        pabort("can't open device");

    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (ret == -1)
        pabort("can't set spi mode");

    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
        pabort("can't set bits per word");

    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
   pabort("can't set max speed hz");
   //printf("spi format::\n");
   //printf("spi mode: %d\n", mode);
   //printf("bits per word: %d\n", bits);
   //printf("spi clock speed:%d KHz (%d MHz)\n", speed/1000,speed/1000000);
   //printf("bitrate: %.3f nanoseconds per bit\n", 1000000000.0/(float)speed);


    StartDisplay(fd);


    spidata_t test[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    size_data = ARRAY_SIZE(test);
    //StartDisplay(fd);



	while(1)
	{
		
        //StartDisplay(fd);
		//transferData(fd, test, size_data);
	}

    close(fd);
	gpio_unexport(DISPL_PWRDWN);
	gpio_unexport(INTERRUPT_DISPL);
    return ret;
}
