/*
 * display.c
 *
 *  Created on: Jun 18, 2014
 *      Author: sander
 */

#include "devices.h"
#include "yo.h"
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


int cmd_button(int fd, int offset, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t font, uint16_t options, const char* s)
{
	cmd(fd, offset, CMD_BUTTON);
	cmd(fd, offset + 4, (((uint32_t)y<<16)|(x & 0xffff)));
	cmd(fd, offset + 8, (((uint32_t)h<<16)|(w & 0xffff)));
	cmd(fd, offset + 12, (((uint32_t)options<<16)|(font & 0xffff)));
	//mem_wr32(fd, RAM_CMD + offset + 16, s);
	//mem_wr16(fd, RAM_CMD + offset + 16 + strlen(s) + 1, 0);

//	printf("%d", (offset + 16 + strlen(s)));
//	if( ((offset + 16 + strlen(s))& 0x03 )!= 0 )
//	{
//		return( ((offset + 16 + strlen(s)) + 4) & 0xFFFC);
//	}
	return (offset + 20);
}

int cmd_toggle(int fd, int offset, uint16_t x, uint16_t y, uint16_t w, uint16_t font, uint16_t options, uint16_t state, const char* s)
{
	cmd(fd, offset, CMD_TOGGLE);
	cmd(fd, offset + 4, (((uint32_t)y<<16)|(x & 0xffff)));
	cmd(fd, offset + 8, (((uint32_t)font<<16)|(w & 0xffff)));
	cmd(fd, offset + 12, (((uint32_t)state<<16)|(options & 0xffff)));
	//mem_wr32(fd, RAM_CMD + offset + 16, s);
	//mem_wr16(fd, RAM_CMD + offset + 16 + strlen(s) + 1, 0);

//	printf("%d", (offset + 16 + strlen(s)));
//	if( ((offset + 16 + strlen(s))& 0x03 )!= 0 )
//	{
//		return( ((offset + 16 + strlen(s)) + 4) & 0xFFFC);
//	}
	return (offset + 20);
}

int cmd_text(int fd, int offset, uint16_t x, uint16_t y, uint16_t font, uint16_t options, const char* s)
{
	//cmd(fd, offset, CMD_TXT);
	cmd(fd, offset + 4, (((uint32_t)y<<16)|(x & 0xffff)));
	cmd(fd, offset + 8, (((uint32_t)options<<16)|(font & 0xffff)));
	//mem_wr32(fd, RAM_CMD + offset + 16, s);
	//mem_wr16(fd, RAM_CMD + offset + 16 + strlen(s) + 1, 0);

//	printf("%d", (offset + 16 + strlen(s)));
//	if( ((offset + 16 + strlen(s))& 0x03 )!= 0 )
//	{
//		return( ((offset + 16 + strlen(s)) + 4) & 0xFFFC);
//	}
	return (offset + 20);
}


/*
int send_string(int fd, uint16_t offset, char *s)
{
	uint16_t length = 0, transfered = 0, count = 0, x;
	length = strlen(s)+1; //+1 voor afsluiting
	count = length;
	do{

		if (length > freespace(fd))
		{
			length = freespace(fd);
		}

		do{
			x = freespace(fd);
		}while(x < length);


	 startcmdtransfer ftgpuwrite, lenght

	 transfered = 0
	 while(length--)
	 {
		 mem_wr8(fd, adress, *s);
		 s++
		 transfered ++
	 }
	 length = transfered;
	 endtransfer
	 update cmdfifo
	 waitcmdfifo empty
	 count -= length
	} while







	return 0;
}

*/


int main()
{
//#define FT800

#ifdef FT800
	export_All_GPIO();//gpio_get_value(INTERRUPT_DISPL);
#endif //FT800

	int spi_fd;
	int x,y,z;

	/* Startup Fase met SPI en Display*/
	if((spi_fd = open_SPI(SPI_MODE, WORD_LENGHT, SPI_SPEED)) < 0 ) printf("SPI not opened\n");
	printf("\nstage1\n");

#ifdef FT800
	if(start_FT800(spi_fd, FT_ACTIVE, FT_CLKEXT, FT_CLK48M) != TRUE) printf("FT800 not properly woken up!\n");
	printf("\nstage2\n");
	if(setup_Screen(spi_fd) != TRUE) printf("Screen dimensions not initialized!\n");
	printf("\nstage3\n");
#endif //FT800
	if(CDS_logo_DL(spi_fd)!=TRUE)printf("\nlalla");

	/*
	while(1)
	{
		if( dl1(spi_fd) != TRUE) printf("faal1\n");
		usleep(500000);
		if( dl2(spi_fd) != TRUE) printf("faal2\n");
		usleep(500000);
		if( dl3(spi_fd) != TRUE) printf("faal3\n");
		usleep(500000);
		if( dl4(spi_fd) != TRUE) printf("faal4\n");
		usleep(500000);
		if( dl5(spi_fd) != TRUE) printf("faal5\n");
		usleep(500000);
		if( dl6(spi_fd) != TRUE) printf("faal6\n");
		usleep(500000);
		if( dl7(spi_fd) != TRUE) printf("faal7\n");
		usleep(500000);
		if( dl8(spi_fd) != TRUE) printf("faal8\n");
		usleep(500000);
		if( dl9(spi_fd) != TRUE) printf("faal9\n");
		usleep(500000);
		if( dl10(spi_fd) != TRUE) printf("faal10\n");
		usleep(500000);
	}
*/

	int offset;
	offset = get_offset(spi_fd);
#define sweeper
#ifdef sweeper
	for( y = 0; y < 240; y=y+40)
	{
		for( x = 0; x < 480; x=x+15)
		{
			dlvar(spi_fd, x, y);
			usleep(100000);
		}
	}
#endif








#ifdef klok
for( x = 0 ; x < 24 ; x++ )
{
	//printf("\n%d", x);
	for( y = 0 ; y < 61 ; y ++)
	{
		for( z = 0; z < 61; z ++)
		{
			offset = get_offset(spi_fd);
			offset = start_cmdDL(spi_fd, offset);
			offset = cmd_clock(spi_fd, offset, 240, 120, 100, 0, x, y, z, 3);
			cmd(spi_fd,  offset  ,DISPLAY_ );
			offset = offset + 4;
			cmd(spi_fd,  offset  ,CMD_SWAP );
			offset = offset + 4;
			mem_wr16(spi_fd, REG_CMD_WRITE , cmdBufferWr + offset);

			usleep(500000);
		}
	}
}
#endif






	while(1)
	{
		int q;
		q = 0;
	}

	close_SPI(spi_fd);
#ifdef FT800
    unexport_All_GPIO();
#endif //FT800
}

