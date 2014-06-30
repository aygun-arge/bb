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



void cmd(int fd, uint32_t offset, uint32_t cmd)
{
	mem_wr32(fd, RAM_CMD + offset, cmd);
}

void cmd_clock(int fd, int offset, uint16_t x, uint16_t y,
				uint16_t r, uint16_t options, uint16_t h,
					uint16_t m, uint16_t s,uint16_t ms){


	mem_wr32(fd, RAM_CMD + offset, CMD_CLOCK);
	mem_wr16(fd, RAM_CMD + offset + 4, x);
	mem_wr16(fd, RAM_CMD + offset + 6, y);
	mem_wr16(fd, RAM_CMD + offset + 8, r);
	mem_wr16(fd, RAM_CMD + offset + 10, options);
	mem_wr16(fd, RAM_CMD + offset + 12, h);
	mem_wr16(fd, RAM_CMD + offset + 14, m);
	mem_wr16(fd, RAM_CMD + offset + 16, s);
	mem_wr16(fd, RAM_CMD + offset + 18, ms);

	int i = 20;
}

/**
 *  \brief Contains functions of drawing a clock by FT800 co-processor
 *  
 *  \param [in] fd      File pointer to spi
 *  \param [in] offset  Offset on the RAM_CMD
 *  \param [in] x       Position X on screen
 *  \param [in] y       Position Y on screen
 *  \param [in] r       Radius of the clock
 *  \param [in] options additional options
 *  \param [in] h       hours
 *  \param [in] m       minutes
 *  \param [in] s       seconds
 *  \param [in] ms      milliseconds
 *  \return Returns the offset created by this function.
 */
int cmd_clock2(int fd, int offset, uint16_t x, uint16_t y, uint16_t r, uint16_t options, uint16_t h, uint16_t m, uint16_t s,uint16_t ms)
{
	cmd(fd, offset, CMD_CLOCK);
	cmd(fd, offset + 4, (((uint32_t)y<<16)|(x & 0xffff)));
	cmd(fd, offset + 8, (((uint32_t)options<<16)|(r & 0xffff)));
	cmd(fd, offset + 12, (((uint32_t)m<<16)|(h & 0xffff)));
	cmd(fd, offset + 16, (((uint32_t)m<<16)|(h & 0xffff)));
	cmd(fd, offset + 20, (((uint32_t)ms<<16)|(s & 0xffff)));

	return 24; //24 voor echte offset
}

int cmd_button(int fd, int offset, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t font, uint16_t options, const char* s)
{
	mem_wr32(fd, RAM_CMD + offset, CMD_BUTTON);
	mem_wr16(fd, RAM_CMD + offset + 4,  x);
	mem_wr16(fd, RAM_CMD + offset + 6,  y);
	mem_wr16(fd, RAM_CMD + offset + 8,  w);
	mem_wr16(fd, RAM_CMD + offset + 10, h);
	mem_wr16(fd, RAM_CMD + offset + 12, font);
	mem_wr16(fd, RAM_CMD + offset + 14, options);
	mem_wr16(fd, RAM_CMD + offset + 16, s);
	mem_wr16(fd, RAM_CMD + offset + 16 + strlen(s) + 1, 0);

	return (16 + strlen(s) + 3);
}

int cmd_button2(int fd, int offset, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t font, uint16_t options, const char* s)
{
	cmd(fd, offset, CMD_BUTTON);
	cmd(fd, offset + 4, (((uint32_t)y<<16)|(x & 0xffff)));
	cmd(fd, offset + 8, (((uint32_t)h<<16)|(w & 0xffff)));
	cmd(fd, offset + 12, (((uint32_t)options<<16)|(font & 0xffff)));
	mem_wr16(fd, RAM_CMD + offset + 16, s);
	mem_wr16(fd, RAM_CMD + offset + 16 + strlen(s) + 1, 0);

	return (16 + strlen(s) + 3);
}

int cmd_fgcolor(int fd, int offset, uint32_t c)
{
	cmd(fd, offset, CMD_FGCOLOR);
	cmd(fd, offset + 4,  c);
	return 8;
}

int cmd_bgcolor(int fd, int offset, uint32_t c)
{
	cmd(fd, offset, CMD_BGCOLOR);
	cmd(fd, offset + 4,  c);
	return 8;
}

int cmd_gradcolor(int fd, int offset, uint32_t c)
{
	cmd(fd, offset, CMD_GRADCOLOR);
	cmd(fd, offset + 4,  c);
	return 8;
}

int cmd_slider(int fd, int offset, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t options, uint16_t val, int16_t range)
{
//0xffffff10
	cmd(fd, offset, CMD_SLIDER);
	cmd(fd, offset + 4, (((uint32_t)y<<16)|(x & 0xffff)));
	cmd(fd, offset + 8, (((uint32_t)h<<16)|(w & 0xffff)));
	cmd(fd, offset + 12, (((uint32_t)val<<16)|(options & 0xffff)));
	mem_wr16(fd, RAM_CMD + offset + 16, range);

	return 18;
}

int cmd_progress(int fd, int n, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t options, uint16_t val, int16_t range)
{
//0xffffff0e
	cmd(fd, offset, CMD_PROGRESS);
	cmd(fd, offset + 4, (((uint32_t)y<<16)|(x & 0xffff)));
	cmd(fd, offset + 8, (((uint32_t)h<<16)|(w & 0xffff)));
	cmd(fd, offset + 12, (((uint32_t)val<<16)|(options & 0xffff)));
	mem_wr16(fd, RAM_CMD + offset + 16, range);

	return 18;
}

int cmd_gauge(int fd, int n, uint16_t x, uint16_t y, uint16_t r, uint16_t options, uint16_t major, uint16_t minor, uint16_t val,uint16_t range)
{
//ffffff13
	cmd(fd, offset, CMD_GAUGE);
	cmd(fd, offset + 4, (((uint32_t)y<<16)|(x & 0xffff)));
	cmd(fd, offset + 8, (((uint32_t)options<<16)|(r & 0xffff)));
	cmd(fd, offset + 12, (((uint32_t)minor<<16)|(major & 0xffff)));
	cmd(fd, offset + 16, (((uint32_t)range<<16)|(val & 0xffff)));
	return 20;
}

int freespace ()
{
	uint16_t fullness, freespace;
	fullness = (2+2);
}


/*****
 *
 *	Start_FT800, starts FT800 chip
 *	returns true of false
 **/
bool start_FT800(int fd, uint8_t pwrMode, uint8_t clkSource, uint8_t clkFreq)
{
	gpio_set_value(DISPL_PWRDWN, LOW);
    usleep(30000);	//Wait 30ms (Should be at least 20ms
    gpio_set_value(DISPL_PWRDWN, HIGH);
    usleep(30000);  //wait 30ms (Should be at least 20ms
    command_write(fd, pwrMode);
    command_write(fd, clkSource);
    command_write(fd, clkFreq);

    if(mem_rd8(fd, 0x102400) == 0x7c)
    {
    	return TRUE;
    }
    return FALSE;
}




void dl(int fd,unsigned long cmd)
{
	//extern DLI;
	mem_wr32(fd, RAM_DL + dli, cmd);
	dli = dli + 4;
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
    mem_wr8(fd, REG_DLSWAP, DLSWAP_FRAME);
}

bool setup_Screen(int fd)
	{
    //instellingen voor AT043B35-15I-10 (WQVGA 480x272)

    mem_wr16(fd, REG_HCYCLE , 548);
    mem_wr16(fd, REG_HOFFSET , 43);
    mem_wr16(fd, REG_HSYNC0 , 0);
    mem_wr16(fd, REG_HSYNC1 , 41);

    mem_wr16(fd, REG_VCYCLE , 292);
    mem_wr16(fd, REG_VOFFSET , 12);
    mem_wr16(fd, REG_VSYNC0 , 0);
    mem_wr16(fd, REG_VSYNC1 , 10);

    mem_wr8(fd, REG_SWIZZLE, 0);
    mem_wr8(fd, REG_PCLK_POL, 1);
    mem_wr8(fd, REG_CSPREAD, 1);

    mem_wr16(fd, REG_VSIZE , 272);
    mem_wr16(fd, REG_HSIZE , 480);

    //eerste displaylist
    mem_wr32(fd, RAM_DL+0, CLEAR_COLOR_RGB(0,0,0));
    mem_wr32(fd, RAM_DL+4, CLEAR(1,1,1));
    mem_wr32(fd, RAM_DL+8, DISPLAY_ );
    mem_wr8(fd, REG_DLSWAP, DLSWAP_FRAME);
    mem_wr8(fd, REG_GPIO_DIR, 0x80);
    mem_wr8(fd, REG_GPIO, 0x80);
    mem_wr8(fd, REG_PCLK, 5);

    return TRUE;
}



int main()
{
	export_All_GPIO();//gpio_get_value(INTERRUPT_DISPL);

	int spi_fd;


	if((spi_fd = open_SPI(SPI_MODE, WORD_LENGHT, SPI_SPEED)) < 0 )
	{
		printf("whoopsie\n");
		//return FALSE;
	}
#ifdef TRUE
	if(start_FT800(spi_fd, FT_ACTIVE, FT_CLKEXT, FT_CLK48M) != TRUE)
	{
		printf("whoopsiedaisy\n");
	}
	if(setup_Screen(spi_fd) != TRUE) printf("Screen not initialised\n");
#endif

	int full;
	int free;
	int CMD_offset;

	cmdBufferRd = mem_rd16(spi_fd, REG_CMD_READ);
	cmdBufferWr = mem_rd16(spi_fd, REG_CMD_WRITE);

	full = (cmdBufferWr - cmdBufferRd) % 4095;
	printf("1, %d\n", full);
	free = (4096 - 4) - full;
	printf("2, %d\n",free);
	CMD_offset = cmdBufferWr;
	printf("%d\n", CMD_offset);


	cmd(spi_fd, CMD_offset ,CMD_DLSTART);
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,CLEAR_COLOR_RGB(1,1,1));
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,CLEAR(1,1,1));
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,COLOR_RGB(255,255,0));
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,POINT_SIZE(400));
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,BEGIN(FTPOINTS));
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,VERTEX2II(238,140, 0, 0));
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,END());
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,DISPLAY_);
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,CMD_SWAP);
	CMD_offset = CMD_offset + 4;
	mem_wr16(spi_fd, REG_CMD_WRITE , CMD_offset);

	cmdBufferRd = mem_rd16(spi_fd, REG_CMD_READ);
	cmdBufferWr = mem_rd16(spi_fd, REG_CMD_WRITE);

	printf("RD %d, WR %d\n", cmdBufferRd, cmdBufferWr);
	int uur = 4, min = 28, sec = 0;


	full = (cmdBufferWr - cmdBufferRd) % 4095;
	free = (4096 - 4) - full;
	CMD_offset = cmdBufferWr;

	cmd(spi_fd, CMD_offset ,CMD_DLSTART);
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,CLEAR_COLOR_RGB(1,1,1));
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,CLEAR(1,1,1));
	CMD_offset = CMD_offset + 4;
	cmd_clock(spi_fd, CMD_offset, 240, 100, 100, 0, 3, 3, 3, 3 );
	CMD_offset = CMD_offset + 20;
	cmd(spi_fd, CMD_offset ,DISPLAY_);
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,CMD_SWAP);
	CMD_offset = CMD_offset + 4;
	mem_wr16(spi_fd, REG_CMD_WRITE , CMD_offset);

	//test met 2 buttons
	cmd(spi_fd, CMD_offset ,CMD_DLSTART);
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,CLEAR_COLOR_RGB(1,1,1));
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,CLEAR(1,1,1));
	CMD_offset = CMD_offset + 4;
	CMD_offset = cmd_button2(spi_fd, CMD_offset, 100, 50, 60, 40, 32, 0, "test");
	CMD_offset = cmd_button2(int fd, CMD_offset, 200, 50, 60, 40, 32, 0, "vier");
	cmd(spi_fd, CMD_offset ,DISPLAY_);
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,CMD_SWAP);
	CMD_offset = CMD_offset + 4;
	mem_wr16(spi_fd, REG_CMD_WRITE , CMD_offset);
	
	while (1)
    {

    }

	close_SPI(spi_fd);
    unexport_All_GPIO();
}

