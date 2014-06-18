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
