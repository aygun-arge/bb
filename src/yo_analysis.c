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

bool dl_presample(int fd)
{
    int dli = 0;

	dli = dl(fd,dli ,CLEAR_COLOR_RGB(10,180,70));//gb color

	dli = dl(fd,dli ,COLOR_RGB(120, 240, 190)); //vlak l color
	dli = dl(fd,dli ,CLEAR( 1, 1, 1));
	dli = dl(fd,dli ,BEGIN(EDGE_STRIP_L));
	dli = dl(fd,dli ,VERTEX2F(240 * 16, 0 * 16));
	dli = dl(fd,dli ,VERTEX2F( 240 * 16 , 230 * 16));
	//dli = dl(fd,dli ,VERTEX2F( 100 * 16 , 240 * 16 ));
	dli = dl(fd,dli ,END());
	dli = dl(fd,dli ,COLOR_RGB(230, 230, 230));
	dli = dl(fd,dli ,BEGIN(EDGE_STRIP_R));
	dli = dl(fd,dli ,VERTEX2F( 240 * 16 , 0 * 16 ));
	dli = dl(fd,dli ,VERTEX2F( 240 * 16 , 230 * 16 ));
	//dli = dl(fd,dli ,VERTEX2F( 320 * 16 , 240 * 16 ));
	dli = dl(fd,dli ,END());
	dli = dl(fd,dli ,COLOR_RGB(255, 255, 255));
	dli = dl(fd,dli ,BEGIN(BITMAPS));
	dli = dl(fd,dli ,VERTEX2II(10,230, 31, 'S') );
	dli = dl(fd,dli ,VERTEX2II(30,230, 31, 'A') );
	dli = dl(fd,dli ,VERTEX2II(50,230, 31, 'M') );
	dli = dl(fd,dli ,VERTEX2II(85,230, 31, 'P') );
	dli = dl(fd,dli ,VERTEX2II(110,230, 31, 'L') );
	dli = dl(fd,dli ,VERTEX2II(130,230, 31, 'I') );
	dli = dl(fd,dli ,VERTEX2II(140,230, 31, 'N') );
	dli = dl(fd,dli ,VERTEX2II(170,230, 31, 'G') );
	dli = dl(fd,dli ,VERTEX2II(190,230, 31, ' ') );
	dli = dl(fd,dli ,VERTEX2II(210,230, 31, 'B') );
	dli = dl(fd,dli ,VERTEX2II(235,230, 31, 'U') );
	dli = dl(fd,dli ,VERTEX2II(260,230, 31, 'S') );
	dli = dl(fd,dli ,VERTEX2II(285,230, 31, 'Y') );

	dli = dl(fd,dli ,END());
	dli = dl(fd,dli ,DISPLAY_);
    mem_wr8(fd, REG_DLSWAP, DLSWAP_FRAME);
    printf("dlbegin setted \n");
    return 1;
}

bool dl_aftersample(int fd, bool result, char fault)
{
    int dli = 0;

    if(result == 1)
    {
    	dli = dl(fd,dli ,CLEAR_COLOR_RGB(10,180,70));//gb color
    	dli = dl(fd,dli ,COLOR_RGB(30, 250, 50));
    	dli = dl(fd,dli ,CLEAR( 1, 1, 1));
    	dli = dl(fd,dli ,BEGIN(EDGE_STRIP_L));
    	dli = dl(fd,dli ,VERTEX2F( 420 * 16, 0 * 16));
    	dli = dl(fd,dli ,VERTEX2F( 420 * 16 , 230 * 16));
    	//dli = dl(fd,dli ,VERTEX2F( 100 * 16 , 240 * 16 ));
    	dli = dl(fd,dli ,END());
    	dli = dl(fd,dli ,COLOR_RGB(230, 250, 250));
    	dli = dl(fd,dli ,BEGIN(EDGE_STRIP_R));
    	dli = dl(fd,dli ,VERTEX2F( 420 * 16 , 0 * 16 ));
    	dli = dl(fd,dli ,VERTEX2F( 420 * 16 , 230 * 16 ));
    	//dli = dl(fd,dli ,VERTEX2F( 320 * 16 , 240 * 16 ));
    	dli = dl(fd,dli ,END());
    }
    else
    {
    	dli = dl(fd,dli ,CLEAR_COLOR_RGB(10,180,70));//gb color
    	dli = dl(fd,dli ,COLOR_RGB(255, 0, 0));
    	dli = dl(fd,dli ,CLEAR( 1, 1, 1));
    	dli = dl(fd,dli ,BEGIN(EDGE_STRIP_L));
    	dli = dl(fd,dli ,VERTEX2F( 90 * 16, 0 * 16));
    	dli = dl(fd,dli ,VERTEX2F( 90 * 16 , 230 * 16));
    	//dli = dl(fd,dli ,VERTEX2F( 100 * 16 , 240 * 16 ));
    	dli = dl(fd,dli ,END());
    	if(fault == 1) dli = dl(fd,dli ,COLOR_RGB(230, 100, 250));
    	if(fault == 2) dli = dl(fd,dli ,COLOR_RGB(100, 250, 50));
    	if(fault == 3) dli = dl(fd,dli ,COLOR_RGB(230, 100, 50));
    	if(fault == 4) dli = dl(fd,dli ,COLOR_RGB(130, 0, 250));

    	dli = dl(fd,dli ,BEGIN(EDGE_STRIP_R));
    	dli = dl(fd,dli ,VERTEX2F( 90 * 16 , 0 * 16 ));
    	dli = dl(fd,dli ,VERTEX2F( 90 * 16 , 230 * 16 ));
    	//dli = dl(fd,dli ,VERTEX2F( 320 * 16 , 240 * 16 ));
    	dli = dl(fd,dli ,END());
    }

    dli = dl(fd,dli ,COLOR_RGB(255, 255, 255));
	dli = dl(fd,dli ,BEGIN(BITMAPS));
	dli = dl(fd,dli ,VERTEX2II(10,230, 31, 'S') );
	dli = dl(fd,dli ,VERTEX2II(30,230, 31, 'A') );
	dli = dl(fd,dli ,VERTEX2II(50,230, 31, 'M') );
	dli = dl(fd,dli ,VERTEX2II(85,230, 31, 'P') );
	dli = dl(fd,dli ,VERTEX2II(110,230, 31, 'L') );
	dli = dl(fd,dli ,VERTEX2II(130,230, 31, 'I') );
	dli = dl(fd,dli ,VERTEX2II(140,230, 31, 'N') );
	dli = dl(fd,dli ,VERTEX2II(170,230, 31, 'G') );
	dli = dl(fd,dli ,VERTEX2II(190,230, 31, ' ') );
	dli = dl(fd,dli ,VERTEX2II(210,230, 31, 'D') );
	dli = dl(fd,dli ,VERTEX2II(235,230, 31, 'O') );
	dli = dl(fd,dli ,VERTEX2II(260,230, 31, 'N') );
	dli = dl(fd,dli ,VERTEX2II(285,230, 31, 'E') );


	dli = dl(fd,dli ,END());
	dli = dl(fd,dli ,DISPLAY_);
    mem_wr8(fd, REG_DLSWAP, DLSWAP_FRAME);
    printf("dlbegin setted \n");
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

int Touch_init(int fd)
{

}

int Get_touch_tag(int fd)
{
	uint8_t currentTag = 0, previousTag = 0, Pendown = 1, currentChar = '|';

	currentTag = mem_rd8(fd, REG_TOUCH_TAG);
	Pendown = ((mem_rd32(fd, REG_TOUCH_DIRECT_XY) >> 31) & 0x01); //reg 32 bevat of screen uberhaupt is aangeraakt
	currentChar = currentTag;
	if( 0 == currentTag)
	{
		currentTag = 1;
	}








}
