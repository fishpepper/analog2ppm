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

#include "led.h"
#include "main.h"

void led_init(){
	DDRB |= (1<<5);
}

void led_toggle(){
	PORTB = PORTB ^ (1<<5);
}

void led_on(){
	PORTB |= (1<<5);
}

void led_off(){
	PORTB &= ~(1<<5);
}

