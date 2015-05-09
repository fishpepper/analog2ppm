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

#include "ppm_out.h"
#include "led.h"
#include "uart.h"

//PPM OUT = PORTB.2 (10) (mandatory, this is OC1B compare ourput)
#define PPM_OUT_PPM_SYNC_PULSE_US 300
#define PPM_OUT_PPM_FRAME_LENGTH_US 20000
#define PPM_OUT_INVERTED 1

void ppm_out_init(){
	//initialise timer 1:
	//we use COMPA and COMPB
	//COMPA triggers the io pin level change
	//COMPB triggers an interrupt and reloads the data whereas
	#if (PPM_OUT_INVERTED)
	TCCR1A = (1<<COM1B1) | (1<<COM1B0) | (1<<WGM11) | (1<<WGM10);
	#else
	TCCR1A = (1<<COM1B1) | (0<<COM1B0) | (1<<WGM11) | (1<<WGM10);
	#endif
	TCCR1B = (1<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10);
	
	//default during reset is 1ms
	OCR1A = ((((F_CPU/1000) * 1000)/1000)/64);
	
	//interrupt for timer value reload is triggered after sync pulse
	OCR1B = ((((F_CPU/1000) * PPM_OUT_PPM_SYNC_PULSE_US)/1000)/64);
	
	//enable interrupts:
	TIMSK1 |= (1<<OCIE1B);
	TIFR1  |= (1<<OCIE1B) |(1<<TOIE1);
	TCNT1 = 0;
	
	//set up pin direction:
	DDRB |= (1<<2);
	PORTB &= ~(1<<2);
	
	
	ppm_out_index = 0;
	
	//init values
	ppm_out_recalc();
}


 
//this isr triggered by COMPB and updates the COMPA value accordingly to the 
//requested ppm width
ISR(TIMER1_COMPB_vect){
	ppm_out_index++;
	
	if (ppm_out_index >= PPM_OUT_CHANNELS+1){
		//overall ppm period
		ppm_out_idletime = ((((F_CPU/1000) * PPM_OUT_PPM_FRAME_LENGTH_US)/1000)/64);
		
		//reset counter
		ppm_out_index = 0;
        }
        
        if(ppm_out_index < PPM_OUT_CHANNELS){
		//we need to output the ppm channels:
		OCR1A = ppm_out_comp[ppm_out_index];
		//substract ppm channel duration from idle timer
		ppm_out_idletime -= OCR1A;
	}else{
		//idle time
		OCR1A = ppm_out_idletime;
		//pre calc values for next round:
		ppm_out_recalc();
	}
	
	//printf("[%2d] = %d\n",ppm_out_index,OCR1A);
	
}

ISR(TIMER1_OVF_vect){
	//nothing to do
}

//calculate ppm phases:
void ppm_out_recalc(){
	/*for(uint8_t i=0; i<PPM_OUT_CHANNELS; i++){
		ppm_out_comp[i] = ((((F_CPU/1000) * 1500)/1000)/64); //1500us
	}*/
	
	//frame time is 1000-2000us:
	for(uint8_t i=0; i<PPM_OUT_CHANNELS; i++){
		uint16_t tmp = 1000 + ppm_out_next[i];
		
		//security checks:
		//make sure we do not leave allowed border
		if (tmp < 1000){
			tmp = 1000;
		}else if (tmp > 2000){
			//safety: set to lowest value!
			tmp = 1000;
		}
		
		//add sync pulse
		tmp += 300;
		
		ppm_out_comp[i] = ((((F_CPU/1000) * tmp)/1000)/64);
	}
	
}

//rescale from 0..255 to 0..1000:
uint16_t ppm_out_rescale(uint8_t in){
	uint16_t rescaled = ((uint16_t)in * 255)/65; //0..1008
	//limit to 1000
	if (rescaled > 1000) rescaled = 1000;
	return rescaled;
}

//update pwm values for the next ppm period:
//input values are 0..255
void ppm_out_set_aetr(uint8_t aeleron, uint8_t elevation, uint8_t throttle, uint8_t roll){
	cli();
	ppm_out_next[0] = ppm_out_rescale(throttle);
	ppm_out_next[1] = ppm_out_rescale(roll);
	ppm_out_next[2] = ppm_out_rescale(aeleron);
	ppm_out_next[3] = ppm_out_rescale(elevation);
	ppm_out_next[4] = 0;
	ppm_out_next[5] = 0;
	sei();
}

