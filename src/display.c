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

void dl(int fd,unsigned long cmd)
{
	memory_write32(fd, RAM_DL + dli, cmd);
	dli = dli + 4;
}

void StartDisplay(int fd)
{
	gpio_set_value(DISPL_PWRDWN, LOW);
    usleep(30000);	//Wait 20ms
    gpio_set_value(DISPL_PWRDWN, HIGH);
    usleep(30000); //wait 20ms
    command_write(fd, FT_ACTIVE);
    command_write(fd, FT_CLKEXT);
    command_write(fd, FT_CLK48M);

    printf("%#08x \n",memory_read8(fd, 0x102400));
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
    dl(fd, DISPLAY());
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
    memory_write32(fd, RAM_DL+8, DISPLAY());
    memory_write8(fd, REG_DLSWAP, DLSWAP_FRAME);
    memory_write8(fd, REG_GPIO_DIR, 0x80);
    memory_write8(fd, REG_GPIO, 0x80);
    memory_write8(fd, REG_PCLK, 5);
}



void main()
{
	//gpio_export(DISPL_PWRDWN);
	//gpio_set_dir(DISPL_PWRDWN, OUTPUT);
	//gpio_export(INTERRUPT_DISPL);
	//gpio_set_dir(INTERRUPT_DISPL, INPUT);
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

StartDisplay(spi_fd);
//usleep(300);
printf("%#08x\n", memory_read8(spi_fd, 0x102400));
printf("%#08x\n", memory_read32(spi_fd, 0x0C0000));

    while (1)
    	{
    	int d = 0;
    	}

        close(spi_fd);
    	gpio_unexport(DISPL_PWRDWN);
    	gpio_unexport(INTERRUPT_DISPL);
        //return ret;




}

