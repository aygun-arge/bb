/*
 ============================================================================
 Name        : LEDtest.c
 Author      : Sander
 Version     :
 Copyright   : CDS Development
 Description : Simpele code waarmee led USR3 wordt geblinkt
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	printf("Flash sessie start nu\n");

	FILE *LEDHandle = NULL; //filepointer
	char *LEDBrightness = "/sys/class/leds/beaglebone:green:usr3/brightness"; //Het betreffende register waar USR3 mee kan worden getoggled

	int i;
	for(i = 1; i < 6 ; i++)
	{
		printf("Flash %d \n", i);
		if((LEDHandle = fopen(LEDBrightness, "r+"))!=NULL) 	//file open in mode[lezen + schrijven]
		{
			fwrite("1", sizeof(char), 1, LEDHandle); 		//schrijf 1 naar register, led usr3 moet aan gaan
			fclose(LEDHandle);
		}
		else printf("1: Kan register niet bereiken\n"); 	//wanneer register niet kan worden gevonden print

		sleep(1); //delay 1 sec

		if((LEDHandle = fopen(LEDBrightness, "r+"))!=NULL) 	//file open in lezen + schrijven
		{
			fwrite("0", sizeof(char), 1, LEDHandle); 		//schrijf 0 naar register, led usr3 moet uit gaan
			fclose(LEDHandle);
		}
		else printf("2: Kan register niet bereiken\n");		//wanneer register niet kan worden gevonden print

		sleep(1);
	}

	printf("Flash sessie stopt\n");

	return 0;
}
