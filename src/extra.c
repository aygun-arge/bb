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

int cmd_fgcolor(int fd, int n, uint32_t c)
{
	mem_wr32(fd, RAM_CMD + n + 0, CMD_FGCOLOR);
	mem_wr32(fd, RAM_CMD + n + 4,  c);
	return (n+8);
}

int cmd_bgcolor(int fd, int n, uint32_t c)
{
	mem_wr32(fd, RAM_CMD + n + 0, CMD_BGCOLOR);
	mem_wr32(fd, RAM_CMD + n + 4,  c);
	return (n+8);
}

int cmd_gradcolor(int fd, int n, uint32_t c)
{
	mem_wr32(fd, RAM_CMD + n + 0, CMD_GRADCOLOR);
	mem_wr32(fd, RAM_CMD + n + 4,  c);
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
	mem_wr32(fd, RAM_DL + DLI, cmd);
	DLI = DLI + 4;
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
