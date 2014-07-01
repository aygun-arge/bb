/*
 * yo_analysis.c
 *
 *  Created on: Jul 1, 2014
 *      Author: sander
 */
#include "spi.h"
#include "FT_GPU.h"
#include "FT800.h"
#include "devices.h"

void audio_play(int fd)
{
	mem_wr16(fd, REG_SOUND, (0x0));
	mem_wr8(fd, REG_PLAY, 1);
}

void audio_stop(int fd)
{
	mem_wr16(fd, REG_SOUND, (0x6C << 8 | 0x41));
	mem_wr8(fd, REG_PLAY, 1);
}

bool dlvar(int fd, short x, short y)
{
    int dli = 0;

    dli = dl(fd,dli ,CLEAR( 1, 1, 1));
    dli = dl(fd,dli, SCISSOR_XY(x,y));
    dli = dl(fd,dli, SCISSOR_SIZE(40,40));
    dli = dl(fd,dli ,CLEAR_COLOR_RGB(255,255,255));
	//dli = dl(fd,dli ,COLOR_RGB(0, 250, 50));
    dli = dl(fd,dli ,CLEAR( 1, 1, 1));

	dli = dl(fd,dli ,DISPLAY_);
    mem_wr8(fd, REG_DLSWAP, DLSWAP_FRAME);
    printf("dlvar setted \n");
    return 1;
}




bool dl1(int fd)
{
    int dli = 0;

    dli = dl(fd,dli ,CLEAR( 1, 1, 1));
    dli = dl(fd,dli, SCISSOR_XY(40,20));
    dli = dl(fd,dli, SCISSOR_SIZE(40,40));
    dli = dl(fd,dli ,CLEAR_COLOR_RGB(255,255,255));
	//dli = dl(fd,dli ,COLOR_RGB(0, 250, 50));
    dli = dl(fd,dli ,CLEAR( 1, 1, 1));

	dli = dl(fd,dli ,DISPLAY_);
    mem_wr8(fd, REG_DLSWAP, DLSWAP_FRAME);
    printf("dl1 setted \n");
    return 1;
}

bool dl2(int fd)
{
    int dli = 0;

    dli = dl(fd,dli ,CLEAR( 1, 1, 1));
    dli = dl(fd,dli, SCISSOR_XY(100,20));
    dli = dl(fd,dli, SCISSOR_SIZE(40,40));
    dli = dl(fd,dli ,CLEAR_COLOR_RGB(255,255,255));
	//dli = dl(fd,dli ,COLOR_RGB(0, 250, 50));
    dli = dl(fd,dli ,CLEAR( 1, 1, 1));

	dli = dl(fd,dli ,DISPLAY_);
    mem_wr8(fd, REG_DLSWAP, DLSWAP_FRAME);
    printf("dl1 setted \n");
    return 1;
}

bool dl3(int fd)
{
    int dli = 0;

    dli = dl(fd,dli ,CLEAR( 1, 1, 1));
    dli = dl(fd,dli, SCISSOR_XY(100,120));
    dli = dl(fd,dli, SCISSOR_SIZE(40,40));
    dli = dl(fd,dli ,CLEAR_COLOR_RGB(255,255,255));
	//dli = dl(fd,dli ,COLOR_RGB(0, 250, 50));
    dli = dl(fd,dli ,CLEAR( 1, 1, 1));

	dli = dl(fd,dli ,DISPLAY_);
    mem_wr8(fd, REG_DLSWAP, DLSWAP_FRAME);
    printf("dl1 setted \n");
    return 1;
}

bool dl4(int fd)
{
    int dli = 0;

    dli = dl(fd,dli ,CLEAR( 1, 1, 1));
    dli = dl(fd,dli, SCISSOR_XY(40,120));
    dli = dl(fd,dli, SCISSOR_SIZE(40,40));
    dli = dl(fd,dli ,CLEAR_COLOR_RGB(255,255,255));
	//dli = dl(fd,dli ,COLOR_RGB(0, 250, 50));
    dli = dl(fd,dli ,CLEAR( 1, 1, 1));

	dli = dl(fd,dli ,DISPLAY_);
    mem_wr8(fd, REG_DLSWAP, DLSWAP_FRAME);
    printf("dl1 setted \n");
    return 1;
}
bool dl5(int fd)
{
    int dli = 0;

    dli = dl(fd,dli ,CLEAR( 1, 1, 1));
    dli = dl(fd,dli, SCISSOR_XY(80,120));
    dli = dl(fd,dli, SCISSOR_SIZE(40,40));
    dli = dl(fd,dli ,CLEAR_COLOR_RGB(255,255,255));
	//dli = dl(fd,dli ,COLOR_RGB(0, 250, 50));
    dli = dl(fd,dli ,CLEAR( 1, 1, 1));

	dli = dl(fd,dli ,DISPLAY_);
    mem_wr8(fd, REG_DLSWAP, DLSWAP_FRAME);
    printf("dl1 setted \n");
    return 1;
}
bool dl6(int fd)
{
    int dli = 0;

    dli = dl(fd,dli ,CLEAR( 1, 1, 1));
    dli = dl(fd,dli, SCISSOR_XY(140,120));
    dli = dl(fd,dli, SCISSOR_SIZE(40,40));
    dli = dl(fd,dli ,CLEAR_COLOR_RGB(255,255,255));
	//dli = dl(fd,dli ,COLOR_RGB(0, 250, 50));
    dli = dl(fd,dli ,CLEAR( 1, 1, 1));

	dli = dl(fd,dli ,DISPLAY_);
    mem_wr8(fd, REG_DLSWAP, DLSWAP_FRAME);
    printf("dl1 setted \n");
    return 1;
}
bool dl7(int fd)
{
    int dli = 0;

    dli = dl(fd,dli ,CLEAR( 1, 1, 1));
    dli = dl(fd,dli, SCISSOR_XY(240,120));
    dli = dl(fd,dli, SCISSOR_SIZE(40,40));
    dli = dl(fd,dli ,CLEAR_COLOR_RGB(255,255,255));
	//dli = dl(fd,dli ,COLOR_RGB(0, 250, 50));
    dli = dl(fd,dli ,CLEAR( 1, 1, 1));

	dli = dl(fd,dli ,DISPLAY_);
    mem_wr8(fd, REG_DLSWAP, DLSWAP_FRAME);
    printf("dl1 setted \n");
    return 1;
}
bool dl8(int fd)
{
    int dli = 0;

    dli = dl(fd,dli ,CLEAR( 1, 1, 1));
    dli = dl(fd,dli, SCISSOR_XY(140,220));
    dli = dl(fd,dli, SCISSOR_SIZE(40,40));
    dli = dl(fd,dli ,CLEAR_COLOR_RGB(255,255,255));
	//dli = dl(fd,dli ,COLOR_RGB(0, 250, 50));
    dli = dl(fd,dli ,CLEAR( 1, 1, 1));

	dli = dl(fd,dli ,DISPLAY_);
    mem_wr8(fd, REG_DLSWAP, DLSWAP_FRAME);
    printf("dl1 setted \n");
    return 1;
}
bool dl9(int fd)
{
    int dli = 0;

    dli = dl(fd,dli ,CLEAR( 1, 1, 1));
    dli = dl(fd,dli, SCISSOR_XY(340,120));
    dli = dl(fd,dli, SCISSOR_SIZE(40,40));
    dli = dl(fd,dli ,CLEAR_COLOR_RGB(255,255,255));
	//dli = dl(fd,dli ,COLOR_RGB(0, 250, 50));
    dli = dl(fd,dli ,CLEAR( 1, 1, 1));

	dli = dl(fd,dli ,DISPLAY_);
    mem_wr8(fd, REG_DLSWAP, DLSWAP_FRAME);
    printf("dl1 setted \n");
    return 1;
}
bool dl10(int fd)
{
    int dli = 0;

    dli = dl(fd,dli ,CLEAR( 1, 1, 1));
    dli = dl(fd,dli, SCISSOR_XY(140,220));
    dli = dl(fd,dli, SCISSOR_SIZE(40,40));
    dli = dl(fd,dli ,CLEAR_COLOR_RGB(255,255,255));
	//dli = dl(fd,dli ,COLOR_RGB(0, 250, 50));
    dli = dl(fd,dli ,CLEAR( 1, 1, 1));

	dli = dl(fd,dli ,DISPLAY_);
    mem_wr8(fd, REG_DLSWAP, DLSWAP_FRAME);
    printf("dl1 setted \n");
    return 1;
}
