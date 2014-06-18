/*
 * spi.h
 *
 *  Created on: Jun 18, 2014
 *      Author: sander
 */

#ifndef SPI_H_
#define SPI_H_

#include "devices.h"

#define ARRAY_SIZE(a) ( (sizeof(a)/sizeof(uint16_t)) * 2) /* lens in bytes */

#define SPI_MODE SPI_MODE_0
#define WORD_LENGHT 8//in bits
#define SPI_SPEED	6000000 //in Hz
#define SPI_DELAY	0


void command_write(int fd, uint8_t cmd );

void memory_write32(int fd, uint32_t Addr, uint32_t Data);
void memory_write16(int fd, uint32_t Addr, uint16_t Data);
void memory_write8(int fd, uint32_t Addr, uint8_t Data);

uint32_t memory_read32(int fd, uint32_t Addr);
uint32_t memory_read16(int fd, uint32_t Addr);
uint32_t memory_read8(int fd, uint32_t Addr);

int SetupSPI(int fd, uint8_t mode, uint8_t bits, uint32_t speed);


#endif /* SPI_H_ */
