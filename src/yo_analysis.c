/*
 * yo_analysis.c
 *
 *  Created on: Jul 1, 2014
 *      Author: sander
 */
#include "spi.h"
#include "yo.h"
#include "FT_GPU.h"
#include "FT800.h"
#include "devices.h"

#define LETTER(X,Y,C) dli = dl(fd, dli, VERTEX2II(X,Y,31,C))

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

bool dl_presample(int fd)
{
    int dli = 0;

    dli = editablevlak(fd, dli, 240, 0, 230, CL_BG, CL_BSYL, CL_BSYR);

	dli = dl(fd,dli ,COLOR_RGB(255, 255, 255));

	dli = dl(fd,dli ,BEGIN(BITMAPS));
   	LETTER(10,  230, 'S');
    LETTER(35,  230, 'A');
    LETTER(60,  230, 'M');
    LETTER(95,  230, 'P');
    LETTER(120, 230, 'L');
    LETTER(145, 230, 'I');
    LETTER(155, 230, 'N');
    LETTER(185, 230, 'G');

    LETTER(220, 230, 'B');
    LETTER(245, 230, 'U');
    LETTER(270, 230, 'S');
    LETTER(295, 230, 'Y');

	dli = dl(fd,dli ,END());
	dli = dl(fd,dli ,DISPLAY_);
    mem_wr8(fd, REG_DLSWAP, DLSWAP_FRAME);
    printf("dlbegin setted \n");
    return 1;
}



bool dl_test(int fd)
{
    int t1,t2 = 130;
    bool rep = 1;

    for(t1 = 0; t1 < 480; t1++)
    {
		int dli = 0;

    	dli = editablevlak(fd,dli, t1, 0, t2, 0xFF00FF, 0x0000FF, 0x00FF00);

    	dli = dl(fd,dli ,DISPLAY_);
        mem_wr8(fd, REG_DLSWAP, DLSWAP_FRAME);

        if(t2 == 272)

        if(rep == 1)
        {
        	t2++;
        }
        if( rep == 0)
        {
        	t2--;
        }
    }
        printf("dlbegin setted \n");
        return 1;
}

bool dl_aftersample(int fd, bool result, char fault)
{
    int dli = 0;

    if(result == 1) dli = editablevlak(fd, dli, 320, 0, 230, CL_BG, CL_OK, 0x339933);
    else
    {
    	if(fault == 1) dli = editablevlak(fd, dli, 120, 0, 230, CL_BG, CL_NOK, CL_LF1);
    	if(fault == 2) dli = editablevlak(fd, dli, 120, 0, 230, CL_BG, CL_NOK, CL_LF2);
    	if(fault == 3) dli = editablevlak(fd, dli, 120, 0, 230, CL_BG, CL_NOK, CL_LF3);
    }


    dli = dl(fd,dli ,COLOR_RGB(255, 255, 255));
	dli = dl(fd,dli ,BEGIN(BITMAPS));

	/*
	 * Tekst in bovenhoek
	 */
	short ypos = 50;
	short xpos = 120;

	if(result == 1)
	{
		LETTER(xpos+10, ypos, 'N' );
		LETTER(xpos+35, ypos, 'O' );

		LETTER(xpos+75, ypos,  'F' );
		LETTER(xpos+95, ypos,  'A' );
		LETTER(xpos+120, ypos, 'I' );
		LETTER(xpos+130, ypos, 'L' );
	}

	if(result == 0)
	{
	LETTER(xpos+10, ypos, 'N' );
	LETTER(xpos+35, ypos, 'O' );

	LETTER(xpos+75, ypos, 'T' );
	LETTER(xpos+100, ypos, 'E' );
	LETTER(xpos+120, ypos, 'R' );
	LETTER(xpos+145, ypos, 'M' );
	LETTER(xpos+178, ypos, 'I' );
	LETTER(xpos+185, ypos, 'N' );
	LETTER(xpos+210, ypos, 'A' );
	LETTER(xpos+230, ypos, 'T' );
	LETTER(xpos+255, ypos, 'I' );
	LETTER(xpos+265, ypos, 'O' );
	LETTER(xpos+295, ypos, 'N' );
	}

	if(fault == 1)
	{
	ypos = 90;
	xpos = 130;
	LETTER(xpos, ypos, 'M' );
	LETTER(xpos+35, ypos, 'A' );
	LETTER(xpos+60, ypos, 'S' );
	LETTER(xpos+85, ypos, 'T' );
	LETTER(xpos+110, ypos, 'E' );
	LETTER(xpos+135, ypos, 'R' );
	}


	if(fault == 2)
	{
		xpos = 130;//solo slave
		ypos = 90;
		LETTER(xpos, ypos, 'S' );
		LETTER(xpos+25, ypos, 'L' );
		LETTER(xpos+50, ypos, 'A' );
		LETTER(xpos+70, ypos, 'V' );
		LETTER(xpos+95, ypos, 'E' );
	}



	if(fault == 3)
	{
		ypos = 90;
		xpos = 130;
		LETTER(xpos, ypos, 'M' );
		LETTER(xpos+35, ypos, 'A' );
		LETTER(xpos+60, ypos, 'S' );
		LETTER(xpos+85, ypos, 'T' );
		LETTER(xpos+110, ypos, 'E' );
		LETTER(xpos+135, ypos, 'R' );

		xpos = 300;
		LETTER(xpos, ypos, '&' );

		xpos = 330;

		LETTER(xpos, ypos, 'S' );
		LETTER(xpos+25, ypos, 'L' );
		LETTER(xpos+50, ypos, 'A' );
		LETTER(xpos+70, ypos, 'V' );
		LETTER(xpos+95, ypos, 'E' );

	}



/*
 * tekst in onderhoek
 */
   	LETTER(10,  230, 'S');
    LETTER(35,  230, 'A');
    LETTER(60,  230, 'M');
    LETTER(95,  230, 'P');
    LETTER(120, 230, 'L');
    LETTER(145, 230, 'I');
    LETTER(155, 230, 'N');
    LETTER(185, 230, 'G');

    LETTER(220, 230, 'D');
    LETTER(245, 230, 'O');
    LETTER(270, 230, 'N');
    LETTER(295, 230, 'E');

	dli = dl(fd,dli ,END());
	dli = dl(fd,dli ,DISPLAY_);
    mem_wr8(fd, REG_DLSWAP, DLSWAP_FRAME);
    printf("dlbegin setted \n");
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


int editablevlak(int fd,int dli, short joint, short yt, short yb, int bgc, int cl, int cr)
{

	dli = dl(fd,dli ,CLEAR_COLOR_RGB(((bgc >> 16 ) & 0xFF),((bgc >> 8 ) & 0xFF	),(bgc & 0xFF) ));//bg color
	dli = dl(fd,dli ,COLOR_RGB(((cl >> 16 ) & 0xFF),((cl >> 8 ) & 0xFF	),(cl & 0xFF) )); //vlak l color
	dli = dl(fd,dli ,CLEAR( 1, 1, 1));
	dli = dl(fd,dli ,BEGIN(EDGE_STRIP_L));
	dli = dl(fd,dli ,VERTEX2F(joint * 16, yt* 16));
	dli = dl(fd,dli ,VERTEX2F( joint * 16 , yb * 16));
	dli = dl(fd,dli ,END());
	dli = dl(fd,dli ,COLOR_RGB(((cr >> 16 ) & 0xFF),((cr >> 8 ) & 0xFF	),(cr & 0xFF) ));
	dli = dl(fd,dli ,BEGIN(EDGE_STRIP_R));
	dli = dl(fd,dli ,VERTEX2F( joint * 16 , yt * 16 ));
	dli = dl(fd,dli ,VERTEX2F( joint * 16 , yb * 16 ));
	dli = dl(fd,dli ,END());

    return dli;
}






