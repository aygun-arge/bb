/*
 * devices.h
 *
 *  Created on: Jun 5, 2014
 *      Author: sander
 */

#ifndef DEVICES_H_
#define DEVICES_H_

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "spi.h"
#include "FT800.h"
#include "FT_GPU.h"
#include <string.h>

#define UART4	"/dev/ttyO4"
#define SPI1	"/dev/spidev1.0"
#define MEM		"/dev/mem"

//PINS

#define PROTOCOL_SELECT 48
#define DISPL_PWRDWN 49
#define INTERRUPT_DISPL 60
#define ADC_BUF 61


#define SYSFS_GPIO_DIR "/sys/class/gpio"
//#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
#define MAX_BUF 64

typedef enum {
INPUT = 0,
OUTPUT = 1
}PIN_DIRECTION;

typedef enum {
LOW = 0,
HIGH = 1
}PIN_VALUE;

typedef enum {
	FNET = 0,
	IONET = 1
}PROTOCOL;


typedef char bool;


#define TRUE 1
#define FALSE 0
/****************************************************************
* gpio_export
****************************************************************/
int gpio_export(unsigned int gpio);
int gpio_unexport(unsigned int gpio);
int gpio_set_dir(unsigned int gpio, PIN_DIRECTION out_flag);
int gpio_set_value(unsigned int gpio, PIN_VALUE value);
int gpio_get_value(unsigned int gpio, unsigned int *value);
int gpio_set_edge(unsigned int gpio, char *edge);
int gpio_fd_open(unsigned int gpio);
int gpio_fd_close(int fd);
void export_All_GPIO();
void unexport_All_GPIO();

void pabort(const char *s);


int handlepru ( char *filename );

#endif /* DEVICES_H_ */
