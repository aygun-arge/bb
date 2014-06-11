/*
* spi.c - Beagle Bone Black ws2811 SPI test driver
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

#include "devices.h"

typedef uint16_t spidata_t;

#define ARRAY_SIZE(a) ( (sizeof(a)/sizeof(spidata_t)) * 2) /* lens in bytes */

static const char *device = "/dev/spidev1.0";
static uint8_t mode = SPI_CPHA;
static uint8_t bits = 15;
static uint32_t speed = 6000000;
static uint16_t delay = 0;

static void pabort(const char *s) {
    perror(s);
    abort();
}

static void transferData(int fd) {
    int ret;

    // data, 16 bits,
    static const spidata_t data[] = {0xAAAA};


    struct spi_ioc_transfer test_spi = {
        .tx_buf = (unsigned long)data,
        .rx_buf = 0, /* null receive data */
        .len = ARRAY_SIZE(data),
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = bits,
    };

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &test_spi);
    if (ret < 1)
        pabort("can't send spi message");

    usleep(50);
}

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
    int ret = 0;
    int fd;

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
    printf("spi format::\n"); 
    printf("spi mode: %d\n", mode);
    printf("bits per word: %d\n", bits);
    printf("spi clock speed:%d KHz (%d MHz)\n", speed/1000,speed/1000000);
    printf("bitrate: %.3f nanoseconds per bit\n", 1000000000.0/(float)speed);

	while(1)
	{
   		transferData(fd);
	}
    close(fd);

    return ret;
}
