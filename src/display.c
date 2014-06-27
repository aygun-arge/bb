/*
 * display.c
 *
 *  Created on: Jun 18, 2014
 *      Author: sander
 */

#include "devices.h"
#include "spi.h"
#include "FT800.h"
#include "FT_GPU.h"

int dli;
uint32_t cmdBufferRd;
uint32_t cmdBufferWr;
int DLI;

#define BUTTON_TEST_XSIZE 200
#define BUTTON_TEST_YSIZE 50

#define BUTTON_TEST1_XLOC	100
#define BUTTON_TEST1_YLOC	100

#define BUTTON_TEST2_XLOC	200
#define BUTTON_TEST2_YLOC	200

#define BUTTON_START_XLOC	200
#define BUTTON_START_YLOC	200

#define BUTTON_START_XSIZE 300
#define BUTTON_START_YSIZE 50

#define BUTTON_TEST_HIGHLVL "high level"
#define BUTTON_TEST_LOWLVL  "low level"








int cmd(int fd, unsigned long cmd, int n)
{
	memory_write32(fd, RAM_CMD + n + 0, cmd);
	return (n+4);
}
/*
void cmd_txt(int fd, )
{

}
*/

int cmd_button(int fd, int n, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t font, uint16_t options, const char* s)
{
	memory_write32(fd, RAM_CMD + n + 0, CMD_BUTTON);
	memory_write16(fd, RAM_CMD + n + 4,  x);
	memory_write16(fd, RAM_CMD + n + 6,  y);
	memory_write16(fd, RAM_CMD + n + 8,  w);
	memory_write16(fd, RAM_CMD + n + 10, h);
	memory_write16(fd, RAM_CMD + n + 12, font);
	memory_write16(fd, RAM_CMD + n + 14, options);
	memory_write16(fd, RAM_CMD + n + 16, s);
	memory_write16(fd, RAM_CMD + n + 16 + strlen(s) + 1, 0);

	return (n + strlen(s) + 3);
}

void cmd_clock(int fd, int n, uint16_t x, uint16_t y, uint16_t r, uint16_t options, uint16_t h, uint16_t m, uint16_t s,uint16_t ms)
{
	printf("c1\n");
memory_write32(fd, RAM_CMD + n + 0, CMD_CLOCK);
printf("c2\n");
memory_write16(fd, RAM_CMD + n + 4,  x);
printf("c4\n");
memory_write16(fd, RAM_CMD + n + 6,  y);
printf("c5\n");
memory_write16(fd, RAM_CMD + n + 8,  r);
printf("c6\n");
memory_write16(fd, RAM_CMD + n + 10, options);
printf("c7\n");
memory_write16(fd, RAM_CMD + n + 12, h);
printf("c8\n");
memory_write16(fd, RAM_CMD + n + 14, m);
printf("c9\n");
memory_write16(fd, RAM_CMD + n + 16, s);
printf("c10\n");
memory_write16(fd, RAM_CMD + n + 18, ms);
printf("c11\n");
printf("%d", (n));
}

int cmd_fgcolor(int fd, int n, uint32_t c)
{
	memory_write32(fd, RAM_CMD + n + 0, CMD_FGCOLOR);
	memory_write32(fd, RAM_CMD + n + 4,  c);
	return (n+8);
}

int cmd_bgcolor(int fd, int n, uint32_t c)
{
	memory_write32(fd, RAM_CMD + n + 0, CMD_BGCOLOR);
	memory_write32(fd, RAM_CMD + n + 4,  c);
	return (n+8);
}

int cmd_gradcolor(int fd, int n, uint32_t c)
{
	memory_write32(fd, RAM_CMD + n + 0, CMD_GRADCOLOR);
	memory_write32(fd, RAM_CMD + n + 4,  c);
	return (n+8);
}
/*
void cmd_gauge(int fd, int n, uint16_t x, uint16_t y, uint16_t r, uint16_t options, uint16_t major, uint16_t minor, uint16_t val,uint16_t range)
{
ffffff13
}

void cmd_gradient(int fd, int n, uint16_t x, uint16_t y, uint32_t rgb0, uint16_t x1, uint16_t y1, uint32_t rgb1)
{
ffffff0b
}

void cmd_keys(int fd, int n, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t font, uint16_t options, const char* s)
{
0xffffff0f
}

void cmd_progress(int fd, int n, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t options, uint16_t val, int16_t range)
{
0xffffff0e
}

void cmd_slider(int fd, int n, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t options, uint16_t val, int16_t range)
{
0xffffff10
}

void cmd_dial(int fd, int n, uint16_t x, uint16_t y, uint16_t r, uint16_t options, uint16_t val, int16_t range)
{
0xffffff10
}

*/
int set_dli(int dli)
{
	static DLI;
	DLI = dli;
	return DLI;
}



void dl(int fd,unsigned long cmd)
{
	extern DLI;
	memory_write32(fd, RAM_DL + DLI, cmd);
	DLI = DLI + 4;
}

/*****
 *
 *	Start_FT800, starts FT800 chip
 *	returns true of false
 **/
char start_FT800(int fd, uint8_t pwrMode, uint8_t clkSource, uint8_t clkFreq)
{
	gpio_set_value(DISPL_PWRDWN, LOW);
    usleep(30000);	//Wait 30ms (Should be at least 20ms
    gpio_set_value(DISPL_PWRDWN, HIGH);
    usleep(30000);  //wait 30ms (Should be at least 20ms
    command_write(fd, pwrMode);
    command_write(fd, clkSource);
    command_write(fd, clkFreq);

    if(memory_read8(fd, 0x102400) == 0x7c)
    {
    	return TRUE;
    }
    return FALSE;
}







void CDS_logo_DL(int fd)
{
    dli = 0;

    dl(fd, CLEAR( 1, 1, 1));
    dl(fd, CLEAR_COLOR_RGB(255,255,255));
    dl(fd, COLOR_RGB(0, 250, 50));
    dl(fd, POINT_SIZE(800));
    dl(fd, BEGIN(FTPOINTS));
    dl(fd, VERTEX2II(238,140, 0, 0));
    dl(fd, END());
    dl(fd, COLOR_RGB(255, 255, 255));
    dl(fd, BEGIN(BITMAPS));
    dl(fd, VERTEX2II(195,110, 31, 'C') );
    dl(fd, VERTEX2II(225,110, 31, 'D') );
    dl(fd, VERTEX2II(255,110, 31, 'S') );
    dl(fd, END());
    dl(fd, DISPLAY_);
    memory_write8(fd, REG_DLSWAP, DLSWAP_FRAME);
}

void setup_screen(int fd)
	{
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
    memory_write32(fd, RAM_DL+0, CLEAR_COLOR_RGB(0,0,0));
    memory_write32(fd, RAM_DL+4, CLEAR(1,1,1));
    memory_write32(fd, RAM_DL+8, DISPLAY_ );
    memory_write8(fd, REG_DLSWAP, DLSWAP_FRAME);
    memory_write8(fd, REG_GPIO_DIR, 0x80);
    memory_write8(fd, REG_GPIO, 0x80);
    memory_write8(fd, REG_PCLK, 5);
}



void main()
{
	const char* k = "hallo";


	gpio_export(DISPL_PWRDWN);
	gpio_set_dir(DISPL_PWRDWN, OUTPUT);
	gpio_export(INTERRUPT_DISPL);
	gpio_set_dir(INTERRUPT_DISPL, INPUT);
	uint32_t p;

	int spi_fd;
    if((spi_fd = open(SPI1, O_RDWR))<0)
    {
    	pabort("Can't open device");
    }

	if(SetupSPI(spi_fd, SPI_MODE, WORD_LENGHT, SPI_SPEED) == FT_FALSE) //setup SPI -> SPI mode 0, 8 bits woord lengte, 6MHz kloksnelheid
    {
    	printf("Init SPI: failed\n");
    	//return 1;
    }
    //gpio_get_value(INTERRUPT_DISPL);

	start_FT800(spi_fd, FT_ACTIVE, CLKEXT, )

	StartDisplay(spi_fd);
	setup_screen(spi_fd);

	cmdBufferRd = memory_read32(spi_fd, REG_CMD_READ);
	cmdBufferWr = memory_read32(spi_fd, REG_CMD_WRITE);


		int x;
		printf("1\n");
		x = cmd(spi_fd, 0, CMD_DLSTART);
		printf("2\n");
		x = cmd(spi_fd, 4, CLEAR_COLOR_RGB(255,255,255));
		printf("3\n");
		x = cmd(spi_fd, 8, CLEAR(60,34,22));
		printf("4\n");
		x = cmd_clock(spi_fd, 12, 100, 120, 50, OPT_NOSECS, 8, 15, 0, 0);
		printf("%d", x);
		//x = cmd(spi_fd, x, DISPLAY_ );

		x = cmd(spi_fd, x, CMD_SWAP );
		memory_write32(spi_fd, REG_CMD_WRITE,cmdBufferWr + x);


	while (1)
    {

    }


    close(spi_fd);
    gpio_unexport(DISPL_PWRDWN);
    gpio_unexport(INTERRUPT_DISPL);
    //return ret;

}

