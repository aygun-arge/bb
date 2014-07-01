/*
 * FT800.c
 *
 *  Created on: Jul 1, 2014
 *      Author: sander
 */
#include "FT800.h"
#include "devices.h"
#include "spi.h"
#include "FT_GPU.h"


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

int dl(int fd, int offset, unsigned long cmd)
{
	mem_wr32(fd, RAM_DL + offset, cmd);
	return (offset + 4);
}

/*
 * Displays CDS basic logo
 */
bool CDS_logo_DL(int fd)
{
    int dli = 0;

    dli = dl(fd,dli ,CLEAR( 1, 1, 1));
	dli = dl(fd,dli ,CLEAR_COLOR_RGB(255,255,255));
	dli = dl(fd,dli ,COLOR_RGB(0, 250, 50));
	dli = dl(fd,dli ,POINT_SIZE(800));
	dli = dl(fd,dli ,BEGIN(FTPOINTS));
	dli = dl(fd,dli ,VERTEX2II(238,140, 0, 0));
	dli = dl(fd,dli ,END());
	dli = dl(fd,dli ,COLOR_RGB(255, 255, 255));
	dli = dl(fd,dli ,BEGIN(BITMAPS));
	dli = dl(fd,dli ,VERTEX2II(195,117, 31, 'C') );
	dli = dl(fd,dli ,VERTEX2II(225,117, 31, 'D') );
	dli = dl(fd,dli ,VERTEX2II(255,117, 31, 'S') );
	dli = dl(fd,dli ,END());
	dli = dl(fd,dli ,DISPLAY_);
    mem_wr8(fd, REG_DLSWAP, DLSWAP_FRAME);
    printf("CDS display setted \n");
    return 1;
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

    printf("screen setup completed\n");
    return TRUE;
}


/* Co-processor specific functions*/

void cmd(int fd, uint32_t offset, uint32_t cmd)
{
	mem_wr32(fd, RAM_CMD + offset, cmd);
}

/*
 * Starts a new CMD DL, returns the created offset
 */
int start_cmdDL(int fd, int offset)
{
	cmd(fd, offset ,CMD_DLSTART);
	cmd(fd, offset + 4,CLEAR_COLOR_RGB(1,1,1));
	cmd(fd, offset + 8,CLEAR(1,1,1));
	return (offset + 12);
}

int freespace (int fd)
{
	uint16_t fullness, freespace, cmdBufferWr, cmdBufferRd;
	cmdBufferRd = mem_rd16(fd, REG_CMD_READ);
	cmdBufferWr = mem_rd16(fd, REG_CMD_WRITE);

	fullness = (cmdBufferWr - cmdBufferRd) % 4095;
	printf("Fullness= %d\n", fullness);
	freespace = (4096 - 4) - fullness;
	printf("Freespace= %d\n",freespace);
	return freespace;
}

uint16_t get_offset(int fd)
{
	uint16_t offset;
	offset = mem_rd16(fd, REG_CMD_WRITE);
	printf("Offset=%d\n", offset);
	return offset;
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
int cmd_clock(int fd, int offset, uint16_t x, uint16_t y, uint16_t r, uint16_t options, uint16_t h, uint16_t m, uint16_t s,uint16_t ms)
{
	cmd(fd, offset, CMD_CLOCK);
	cmd(fd, offset + 4, (((uint32_t)y<<16)|(x & 0xffff)));
	cmd(fd, offset + 8, (((uint32_t)options<<16)|(r & 0xffff)));
	cmd(fd, offset + 12, (((uint32_t)m<<16)|(h & 0xffff)));
	cmd(fd, offset + 16, (((uint32_t)ms<<16)|(s & 0xffff)));
	return (offset + 20);
}

int cmd_fgcolor(int fd, int offset, uint32_t c)
{
	cmd(fd, offset, CMD_FGCOLOR);
	cmd(fd, offset + 4,  c);
	return (offset + 8);
}

int cmd_bgcolor(int fd, int offset, uint32_t c)
{
	cmd(fd, offset, CMD_BGCOLOR);
	cmd(fd, offset + 4,  c);
	return (offset + 8);
}

int cmd_gradcolor(int fd, int offset, uint32_t c)
{
	cmd(fd, offset, CMD_GRADCOLOR);
	cmd(fd, offset + 4,  c);
	return (offset + 8);
}

int cmd_gradient(int fd, int offset, uint16_t x0, uint16_t y0, uint32_t rgb0, uint16_t x1, uint16_t y1, uint32_t rgb1)
{
	cmd(fd, offset, CMD_GRADIENT);
	cmd(fd, offset + 4, (((uint32_t)y0<<16)|(x0 & 0xffff)));
	cmd(fd, offset + 8, rgb0);
	cmd(fd, offset + 12, (((uint32_t)y1<<16)|(x1 & 0xffff)));
	cmd(fd, offset + 16, rgb1);
	return (offset + 20); //20 voor echte offset
}

int cmd_slider(int fd, int offset, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t options, uint16_t val, uint16_t range)
{
	cmd(fd, offset, CMD_SLIDER);
	cmd(fd, offset + 4, (((uint32_t)y<<16)|(x & 0xffff)));
	cmd(fd, offset + 8, (((uint32_t)h<<16)|(w & 0xffff)));
	cmd(fd, offset + 12, (((uint32_t)val<<16)|(options & 0xffff)));
	mem_wr16(fd, RAM_CMD + offset + 16, range);

	return (offset + 20);
}

int cmd_progress(int fd, int offset, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t options, uint16_t val, uint16_t range)
{
	cmd(fd, offset, CMD_PROGRESS);
	cmd(fd, offset + 4, (((uint32_t)y<<16)|(x & 0xffff)));
	cmd(fd, offset + 8, (((uint32_t)h<<16)|(w & 0xffff)));
	cmd(fd, offset + 12, (((uint32_t)val<<16)|(options & 0xffff)));
	mem_wr16(fd, RAM_CMD + offset + 16, range);
	return (offset + 20);
}

int cmd_gauge(int fd, int offset, uint16_t x, uint16_t y, uint16_t r, uint16_t options, uint16_t major, uint16_t minor, uint16_t val,uint16_t range)
{
	cmd(fd, offset, CMD_GAUGE);
	cmd(fd, offset + 4, (((uint32_t)y<<16)|(x & 0xffff)));
	cmd(fd, offset + 8, (((uint32_t)options<<16)|(r & 0xffff)));
	cmd(fd, offset + 12, (((uint32_t)minor<<16)|(major & 0xffff)));
	cmd(fd, offset + 16, (((uint32_t)range<<16)|(val & 0xffff)));
	return (offset + 20);
}

