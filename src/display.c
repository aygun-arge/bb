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


	/*

	mem_wr32(spi_fd, RAM_CMD + CMD_offset, CMD_DLSTART);
	CMD_offset = CMD_offset + 4;
	printf("%d\n", CMD_offset);
	mem_wr32(spi_fd, RAM_CMD + CMD_offset, CLEAR_COLOR_RGB(1,1,1));
	CMD_offset = CMD_offset + 4;
	printf("%d\n", CMD_offset);
	mem_wr32(spi_fd, RAM_CMD + CMD_offset, CLEAR(1,1,1));
	printf("lala\n");

	cmd_clock(spi_fd, 100, 120, 50, OPT_NOSECS, 8, 15, 0, 0);

	CMD_offset = mem_rd16(spi_fd, REG_CMD_WRITE);

	mem_wr32(spi_fd, RAM_CMD + CMD_offset, DISPLAY_);
	CMD_offset = CMD_offset + 4;
	printf("%d\n", CMD_offset);
	mem_wr32(spi_fd, RAM_CMD + CMD_offset, CMD_SWAP);
	CMD_offset = CMD_offset + 4;
	printf("%d\n", CMD_offset);
	mem_wr16(spi_fd, REG_CMD_WRITE , CMD_offset);
*/
	while (1)
    {

    }

	close_SPI(spi_fd);
    unexport_All_GPIO();
}

