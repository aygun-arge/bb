#!/bin/bash

#verwijder hekjes voor instellen device trees

cat $SLOTS

#UART van PRU
#echo BB-BONE-PRU-UART-CDS > $SLOTS

#ADC pinnen pru
echo BB-BONE-PRU-S3 > $SLOTS

#uart4
echo BB-UART4 > $SLOTS

#SPI1
echo BB-SPIDEV1-S1 > $SLOTS

cat $SLOTS
