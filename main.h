#ifndef MAIN_H__
#define MAIN_H__

///BAUDRATE:
#define BAUD 57600UL

#define F_CPU 16000000UL
#define F_OSC F_CPU

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/parity.h>

#define max(a, b) (( ( a > b) ? a : b ) )
#define min(a, b) (( ( a < b) ? a : b ) )
#define delay_ms(a) { if(a<(262140000/F_CPU)) _delay_ms(a); else for (unsigned int my_i=0;my_i<a;my_i++) _delay_ms(1); }

#endif

