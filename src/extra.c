/*
 * extra.c
 *
 *  Created on: Jun 27, 2014
 *      Author: sander
 */

#include "devices.h"
#include "spi.h"
#include "FT800.h"
#include "FT_GPU.h"




/*
void cmd_gauge(int fd, int n, uint16_t x, uint16_t y, uint16_t r, uint16_t options, uint16_t major, uint16_t minor, uint16_t val,uint16_t range)
{
ffffff13
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

/*
void cmd_txt(int fd, )
{

}
*/

int cmd_button(int fd, int n, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t font, uint16_t options, const char* s)
{
	mem_wr32(fd, RAM_CMD + n + 0, CMD_BUTTON);
	mem_wr16(fd, RAM_CMD + n + 4,  x);
	mem_wr16(fd, RAM_CMD + n + 6,  y);
	mem_wr16(fd, RAM_CMD + n + 8,  w);
	mem_wr16(fd, RAM_CMD + n + 10, h);
	mem_wr16(fd, RAM_CMD + n + 12, font);
	mem_wr16(fd, RAM_CMD + n + 14, options);
	mem_wr16(fd, RAM_CMD + n + 16, s);
	mem_wr16(fd, RAM_CMD + n + 16 + strlen(s) + 1, 0);

	return (n + strlen(s) + 3);
}


/*
	int x1 = 0, x2, x3;
	char str1[]= {'x', 'y', 'z'};
	//char str2[]= {'w', 'v', 'q'};
	char *p;
	int x;
	p = "hallo ik ben een tekst in een pointer";
	printf("string lengte: %d\n", strlen(p));
	printf("String inhoud: %s\n", p);
	x = strlen(p);
	x1 = strlen(str1);
	char buffer[(x + x1 + 1)];
	printf("size of buffer: %d\n", strlen(buffer));
	strcpy(buffer, str1);
	printf("String inhoud buffer: %s\n", buffer);
	strcat(buffer, p);
	printf("String inhoud buffer: %s\n", buffer);
	printf("%d\n", x);
	char wrdata[(x + x1 + 1)];
	wrdata[0] = 'a';
	wrdata[1] = 'b';
	wrdata[2] = 'c';
	printf("%s", wrdata);
	for(x1 = x+3; x1 < 3; x1-- )
	{
		printf("\n%d, %d\n", x1, x2);
		wrdata[x1] = buffer[x2];
	}
	printf("%s", wrdata);
*/

void bla(){
	int CMD_offset, free;
	free = freespace(spi_fd);
	CMD_offset = get_offset(spi_fd);

	CMD_offset = start_cmdDL(spi_fd, CMD_offset);
	CMD_offset = cmd_bgcolor(spi_fd, CMD_offset, 0xff0000);
	printf("%d\n", CMD_offset);
	CMD_offset = cmd_clock(spi_fd, CMD_offset, 100, 50, 40, OPT_NOSECS, 3,3,3,3);//rood
	printf("%d\n", CMD_offset);
	CMD_offset = cmd_bgcolor(spi_fd, CMD_offset, 0x0000ff);
	printf("%d\n", CMD_offset);
	CMD_offset = cmd_clock(spi_fd, CMD_offset, 200, 50, 40, OPT_NOSECS, 9,9,40,3);//groen
	CMD_offset = cmd_bgcolor(spi_fd, CMD_offset, 0x00ff00);
	printf("%d\n", CMD_offset);
	CMD_offset = cmd_clock(spi_fd, CMD_offset, 300, 50, 40, OPT_NOSECS, 0,12,12,3);//blauw

	CMD_offset = cmd_bgcolor(spi_fd, CMD_offset, 0x00ff00);
	CMD_offset = cmd_progress(spi_fd, CMD_offset, 50, 100, 200, 10, OPT_FLAT, 10, 100);

	CMD_offset = cmd_bgcolor(spi_fd, CMD_offset, 0xff0000);
	CMD_offset = cmd_slider(spi_fd, CMD_offset, 250, 150, 120, 3, 0, 30, 100);



	CMD_offset = cmd_button(spi_fd, CMD_offset, 100, 50, 60, 40, 32, 0, 0);


	//CMD_offset = CMD_offset + 20;
	cmd(spi_fd, CMD_offset ,DISPLAY_);
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,CMD_SWAP);
	CMD_offset = CMD_offset + 4;
	printf("%d\n", CMD_offset);
	mem_wr16(spi_fd, REG_CMD_WRITE , cmdBufferWr + CMD_offset);

	cmdBufferRd = mem_rd16(spi_fd, REG_CMD_READ);
	cmdBufferWr = mem_rd16(spi_fd, REG_CMD_WRITE);

	printf("RD %#x, WR %#x\n", cmdBufferRd, cmdBufferWr);


	/*
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,POINT_SIZE(400));
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,BEGIN(FTPOINTS));
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,VERTEX2II(238,140, 0, 0));
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,END());
	CMD_offset = CMD_offset + 4;
	*/



	//cmdBufferRd = mem_rd16(spi_fd, REG_CMD_READ);
	//cmdBufferWr = mem_rd16(spi_fd, REG_CMD_WRITE);

	//printf("RD %#x, WR %#x\n", cmdBufferRd, cmdBufferWr);


	//full = (cmdBufferWr - cmdBufferRd) % 4095;
	//free = (4096 - 4) - full;
	//CMD_offset = cmdBufferWr;

	//test met 2 buttons
	/*
	cmd(spi_fd, CMD_offset ,CMD_DLSTART);
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,CLEAR_COLOR_RGB(1,1,1));
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,CLEAR(1,1,1));
	CMD_offset = CMD_offset + 4;
	CMD_offset = cmd_button2(spi_fd, CMD_offset, 100, 50, 60, 40, 32, 0, "test");
	CMD_offset = cmd_button2(spi_fd, CMD_offset, 200, 50, 60, 40, 32, 0, "vier");
	cmd(spi_fd, CMD_offset ,DISPLAY_);
	CMD_offset = CMD_offset + 4;
	cmd(spi_fd, CMD_offset ,CMD_SWAP);
	CMD_offset = CMD_offset + 4;

	*/
	//mem_wr16(spi_fd, REG_CMD_WRITE , CMD_offset);
}






