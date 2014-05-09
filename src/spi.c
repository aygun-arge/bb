/* SPI.c	*/
/* sander	*/
/*		*/

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

int ReadSPI(void);

int main(void)
{
	printf("spi test programma (meten op scope\n\n");

	int SPI1_D1;	//FilePointer SPI1_D1	

	int bufout = 0;//random waarde om op scope te bekijken

	if((SPI1_D1  = open("/dev/spidev1.0", O_RDWR)) != NULL)
	{
		while(1)
		{
			write(SPI1_D1, &bufout, 1);
			printf("**CHECK --> %d **\n", ReadSPI());		
			bufout++;
			if(bufout >= 100) bufout = 0;
		}
	
		close(SPI1_D1);	
	
		printf("\n END, je zou dit niet moeten zien\n\n");
	}
	else 
	{
		printf("kan spi1_d1 niet openen\n");
	}
	return 0;
}

int ReadSPI(void)
{
	int SPI1_D0;
	int bufin = 10;	

	
	if((SPI1_D0 = open("/dev/spidev1.1", O_R
DONLY)) != NULL)
	{
		bufin = 
		read(SPI1_D0, &bufin, 1);

		close(SPI1_D0);
	}
	else
	{
		printf("kan spi_d0 niet openen\n");
	}
	return bufin;
}










