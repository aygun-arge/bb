/*
 * yo.h
 *
 *  Created on: Jul 1, 2014
 *      Author: sander
 */

#ifndef YO_H_
#define YO_H_

void audio_play(int fd);
void audio_stop(int fd);

bool dlvar(int fd, short x, short y);
bool dl_presample(int fd);
bool dl_aftersample(int fd, bool result, char fault);
int editablevlak(int fd,int dli, short joint, short yt, short yb, int bgc, int cl, int cr);
bool dl_test(int fd);

/*Colormacros*/
#define CL_BG 0xD6C2AD
#define CL_OK 0x19A319
#define CL_NOK 0xFF0000
#define CL_LF1 0xFF9933
#define CL_LF2 0xFF0066
#define CL_LF3 0xCC00FF
#define CL_FNET 0x336600
#define CL_IONET 0x339966
#define CL_CK_SL 0x3399FF
#define CL_BSYL 0xB2F0B2
#define CL_BSYR 0xFFB2B2







#endif /* YO_H_ */

