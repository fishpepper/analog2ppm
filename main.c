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
#include "main.h"
#include "ppm_out.h"
#include "adc.h"
#include "led.h"

//arduino pinout: http://www.jameco.com/Jameco/workshop/JamecoBuilds/arduinocircuit_fig12.jpg

//stdout to uart
static FILE mystdout = FDEV_SETUP_STREAM( uart_putc_s, NULL, _FDEV_SETUP_WRITE );

int main(void) {
	
	led_init();
	uart_init();
	
	stdout = &mystdout;
	printf("analog2ppm\n");
	ppm_out_init();
	
	//enable ints
	sei();
	
	//enable adc with ints enabled
	adc_init();

	while(1){
		printf("ADC IN: "); for(uint8_t i=0; i<8; i++) printf("%3d ",adc_data[i]); printf("\n");
		
		ppm_out_set_aetr(adc_data[0], adc_data[1], adc_data[2], adc_data[3]);
		
		//loop runs with roughly 100hz:
		delay_ms(10);
	}
	
}

