/*
 * spi.h
 *
 *  Created on: Jun 18, 2014
 *      Author: sander
 */

#ifndef SPI_H_
#define SPI_H_

#include "devices.h"

#define DATAFORMAT_LITTLE_ENDIAN 1

#define ARRAY_SIZE(a) ( (sizeof(a)/sizeof(uint16_t)) * 2) /* lens in bytes */

#define SPI_MODE SPI_MODE_0
#define WORD_LENGHT 8//in bits
#define SPI_SPEED	6000000 //in Hz
#define SPI_DELAY	0

void command_write(int fd, uint8_t cmd );

void mem_wr32(int fd, uint32_t Addr, uint32_t Data);
void mem_wr16(int fd, uint32_t Addr, uint16_t Data);
void mem_wr8(int fd, uint32_t Addr, uint8_t Data);

uint32_t mem_rd32(int fd, uint32_t Addr);
uint32_t mem_rd16(int fd, uint32_t Addr);
uint32_t mem_rd8(int fd, uint32_t Addr);

int open_SPI(uint8_t spi_mode, uint8_t word_size, uint32_t spi_speed);
bool close_SPI(int fd);

#endif /* SPI_H_ */
