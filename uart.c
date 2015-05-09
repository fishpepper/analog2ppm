/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

   author: fishpepper <AT> gmail.com
*/
#include "uart.h"

void uart_init(void){
    //enable uart tx, async 8N1:
    UCSR0B |= (1<<TXEN0);
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
    
    //enable uart rx
    #define UBRR (((F_OSC)/(8*BAUD))-1)

    UCSR0A = (1<<U2X0);    
    UBRR0 = UBRR;
}


void uart_putc(uint8_t c){
    if (c=='\n') uart_putc('\r');
    while (!(UCSR0A & (1<<UDRE0))){}
    UDR0 = c;
}

int uart_putc_s(char c, FILE *stream){
	uart_putc(c);
	return 0;
}
 
void uart_puts(uint8_t *s){
    while (*s){
        uart_putc(*s);
        s++;
    }
}

uint8_t uart_getc(uint8_t *val){
    if (!(UCSR0A & (1<<RXC0))){
	//no new datza
	return 0;
    }
    *val = UDR0;
    return 1;
}

