/*
 * FT800.h
 *
 *  Created on: Jun 18, 2014
 *      Author: sander
 *      Bevat veel uit:
 *      FT_GPU_HAL.h
 *      FT_HAL_UTILS.h
 *      FT_DATATYPES
 *
 */

#ifndef FT800_H_
#define FT800_H_

//Alle mogelijke commando's volgens AN240 (2 MSB bits al goed gezet)

//Power modes
#define FT_ACTIVE	0x00
#define FT_STANDBY	0x41
#define FT_SLEEP	0x42
#define FT_PWRDOWN	0x50

//Clock Switching
//Type
#define FT_CLKINT	0x48
#define FT_CLKEXT	0x44
//Speed
#define FT_CLK24M	0x64
#define FT_CLK36M	0x61
#define FT_CLK48M	0x62

//Miscellaneous
#define FT_CORERST  0x68

//FT_GPU_HAL.h

typedef enum {
	FT_GPU_I2C_MODE = 0,
	FT_GPU_SPI_MODE,

	FT_GPU_MODE_COUNT,
	FT_GPU_MODE_UNKNOWN = FT_GPU_MODE_COUNT
}FT_GPU_HAL_MODE_E;

typedef enum {
	FT_GPU_HAL_OPENED,
	FT_GPU_HAL_READING,
	FT_GPU_HAL_WRITING,
	FT_GPU_HAL_CLOSED,

	FT_GPU_HAL_STATUS_COUNT,
	FT_GPU_HAL_STATUS_ERROR = FT_GPU_HAL_STATUS_COUNT
}FT_GPU_HAL_STATUS_E;

typedef enum {
	FT_GPU_READ = 0,
	FT_GPU_WRITE,
}FT_GPU_TRANSFERDIR_T;


//FT_HAL_UTILS
#define RGB(r, g, b)  ((((vc_int32_t)(r)) << 16) | (((vc_int32_t)(g)) << 8) | (b))
#define SQ(v) ((v) * (v))
#define MIN(x,y)  ((x) > (y) ? (y) : (x))
#define MAX(x,y)  ((x) > (y) ? (x) : (y))
#define PLAYCOLOR        0x00A0A080
#define NOTE(n, sharp)   (((n) - 'C') + ((sharp) * 128))
#define F16(s)           ((vc_int32_t)((s) * 65536))
#define INVALID_TOUCH_XY   0x8000
#define ABS(x)  ((x) > (0) ? (x) : (-x))

//iets
#define FT_FALSE           0
#define FT_TRUE            1

//FT_DATATYPES
/* Inclusion of datatypes from MSVC */
typedef char ft_char8_t;
typedef signed char ft_schar8_t;
typedef unsigned char ft_uchar8_t;
typedef ft_uchar8_t ft_uint8_t;
typedef short  ft_int16_t;
typedef unsigned short ft_uint16_t;
typedef unsigned int ft_uint32_t;
typedef int ft_int32_t;
typedef void ft_void_t;
typedef long long ft_int64_t;
typedef unsigned long long ft_uint64_t;
typedef float ft_float_t;
typedef double ft_double_t;
typedef char ft_bool_t;

#define FT_BYTE_SIZE (1)
#define FT_SHORT_SIZE (2)
#define FT_WORD_SIZE (4)
#define FT_DWORD_SIZE (8)

#define FT_NUMBITS_IN_BYTE (1*8)
#define FT_NUMBITS_IN_SHORT (2*8)
#define FT_NUMBITS_IN_WORD (4*8)
#define FT_NUMBITS_IN_DWORD (8*8)

#define ft_prog_uchar8_t  ft_uchar8_t
#define ft_prog_char8_t   ft_char8_t
#define ft_prog_uint16_t  ft_uint16_t


#define ft_random(x)		(rand() % (x))
#define ft_millis()         GetTickCount()

#define ft_pgm_read_byte_near(x)   (*(x))
#define ft_pgm_read_byte(x)        (*(x))

#define ft_strcpy_P     strcpy
#define ft_strlen_P     strlen

#define FT_DBGPRINT(x)  printf(x)
#define FT_PROGMEM

#define ft_pgm_read_byte_near(x)   (*(x))
#define ft_pgm_read_byte(x)        (*(x))

#define ft_pgm_read_word(addr)   (*(ft_int16_t*)(addr))


#endif /* FT800_H_ */
