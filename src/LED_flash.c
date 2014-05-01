/*
 ============================================================================
 Name        : BBB.c
 Author      : Sander
 Version     :
 Copyright   : CDS Development
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	printf("flash start\n");

	FILE *LEDHandle = NULL; //filepointer
	char *LEDBrightness = "/sys/class/leds/beaglebone:green:usr3/brightness"; //Het register waar iets naar toe moet worden geschreven

	int i;
	for(i = 0; i < 11 ; i++)
	{
		printf("flash %d \n", i);
		if((LEDHandle = fopen(LEDBrightness, "r+"))!=NULL) //file open in lezen + schrijven
		{
			fwrite("1", sizeof(char), 1, LEDHandle); //schrijf 1 naar registertje, led usr3 moet aan gaan
			fclose(LEDHandle);
		}
		else printf("nope1"); //wanneer register niet kan worden gevonden print nope1

		sleep(1); //delay 1 sec

		if((LEDHandle = fopen(LEDBrightness, "r+"))!=NULL) //file open in lezen + schrijven
		{
			fwrite("0", sizeof(char), 1, LEDHandle); //schrijf 0 naar registertje, led usr3 moet uit gaan
			fclose(LEDHandle);
		}
		else printf("nope2");//wanneer register niet kan worden gevonden print nope2

		sleep(1);
	}

	printf("flash stop\n");



	return 0;
}
