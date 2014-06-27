/*
 * startup.c
 *
 *  Created on: Jun 18, 2014
 *      Author: sander
 */

#include "devices.h"

void pabort(const char *s)
{
    perror(s);
    abort();
}

void export_All_GPIO()
{
	gpio_export(DISPL_PWRDWN);
	gpio_export(INTERRUPT_DISPL);
	gpio_export(ADC_BUF);
	gpio_export(PROTOCOL_SELECT);

	gpio_set_dir(DISPL_PWRDWN, OUTPUT);
	gpio_set_dir(INTERRUPT_DISPL, INPUT);
	gpio_set_dir(ADC_BUF, OUTPUT);
	gpio_set_dir(PROTOCOL_SELECT, OUTPUT);

	gpio_set_value(ADC_BUF, HIGH);
	gpio_set_value(DISPL_PWRDWN, HIGH);
	gpio_set_value(PROTOCOL_SELECT, IONET);

}

void unexport_All_GPIO()
{
	gpio_unexport(DISPL_PWRDWN);
	gpio_unexport(INTERRUPT_DISPL);
	gpio_unexport(ADC_BUF);
	gpio_unexport(PROTOCOL_SELECT);
}

