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
#include "adc.h"

void adc_init(void){
	//init adc
	ADCSRA = 0;
	ADCSRB = 0;
	
	ADMUX  = (0 << REFS1) | (1<<REFS0); //set reference voltage Avcc with cap on AREF
	ADMUX |= (1 << ADLAR); //left align the ADC value (ADCH = 8bit value)
	
	ADCSRA = (1 << ADPS2) | (1<<ADPS1) | (1 << ADPS0); //clk divider 128
	ADCSRA |= (0 << ADATE); //auto trigger
	ADCSRA |= (1 << ADIE); //enable interrupts when measurement complete
	ADCSRA |= (1 << ADEN); //enable ADC

	ADCSRB = 0;
	
	//start first conversion:
	ADCSRA |= (1 << ADSC);
}


ISR(ADC_vect) {//when new ADC value ready
	//store data at current channel id:
	uint8_t channel = (ADMUX & 0x07);
	adc_data[channel] = ADCH;

	//next channel:
	ADMUX = (ADMUX & 0xF0) | ((channel+1)&0x07);

	//start next conversion:
	ADCSRA |= (1 << ADSC);
}
