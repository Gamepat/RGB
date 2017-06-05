/*
 * RGB-Control.c
 *
 * @date: 10.04.2017 18:58:26
 * @author: Lengauer Jakob
 */ 

/* TODOs:
 * advanced color changer
 */

/*
 * Information:
 * Brightness from 0 - 255
 * OCR1A	Pin11	connect RED
 * OCR3A	Pin5	connect GREEN
 * OCR4A	Pin6	connect BLUE
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART.h"


void color(int red, int green, int blue) {
	OCR1A = red;
	OCR3A = green;
	OCR4A = blue;
}

void delay(int t) {
	for(int i = 0; i < t; i++) {
		_delay_ms(1);
	}
}

/*
 * Converts the input chars into an Integer
 */
int converter(char enter) {
	int ret = 0;
	switch(enter) {
		case '0': ret = 0; break;
		case '1': ret = 1; break;
		case '2': ret = 2; break;
		case '3': ret = 3; break;
		case '4': ret = 4; break;
		case '5': ret = 5; break;
		case '6': ret = 6; break;
		case '7': ret = 7; break;
		case '8': ret = 8; break;
		case '9': ret = 9; break;
		case 'a': ret = 10; break;
		case 'b': ret = 11; break;
		case 'c': ret = 12; break;
		case 'd': ret = 13; break;
		case 'e': ret = 14; break;
		case 'f': ret = 15; break;
		case 'g': ret = 16; break;
		case 'h': ret = 17; break;
		case 'i': ret = 18; break;
		case 'j': ret = 19; break;
		case 'k': ret = 20; break;
		case 'l': ret = 21; break;
		case 'm': ret = 22; break;
		case 'n': ret = 23; break;
		case 'o': ret = 24; break;
		case 'p': ret = 25; break;
		case 'q': ret = 26; break;
		case 'r': ret = 27; break;
		case 's': ret = 28; break;
		case 't': ret = 29; break;
		case 'u': ret = 30; break;
		case 'v': ret = 31; break;
		case 'w': ret = 32; break;
		case 'x': ret = 33; break;
		case 'y': ret = 34; break;
		case 'z': ret = 35; break;
		default: break;
	}
	return ret;
}

/*
 * Single Color Mode 
 */
void singeColor(int number) {
	
	switch(number) {
		/*TODO Make the colors more accurate*/
		case 0: color(0, 0, 0); break;				//off
		
		case 1: color(255, 0, 0); break;			//red
		case 2: color(83, 4, 0); break;				//orange1
		case 3: color(150, 12, 0); break;			//orange2
		case 4: color(210, 90, 0); break;			//yellow1
		case 5: color(255, 140, 0); break;			//yellow
		
		case 6: color(0, 255, 0); break;			//green
		case 7: color(85, 107, 47); break;			//olive green
		case 8: color(34, 139, 34); break;			//forest green
		case 9: color(50, 205, 50); break;			//lime green
		case 10: color(0, 255, 255); break;			//cyan
		
		case 11: color(0, 0, 255); break;			//blue
		case 12: color(24, 116, 205); break;		//dodge
		case 13: color(25, 25, 112); break;			//midnight blue
		case 14: color(71, 60, 139); break;			//slate
		case 15: color(255, 0, 255); break;			//purple
		
		case 16: color(255, 255, 255); break;		//white
		case 17: color(255, 187, 255); break;		//plum
		case 18: color(214, 102, 255); break;		//orchid1
		case 19: color(191, 62, 255); break;		//orchid2
		case 20: color(139, 0, 139); break;			//magenta
		default: break;
	}
}

/*
 * Color Fade Mode
 */
int fadeColor(int number) {
	
	int fade = 0;
	switch(number) {
		case 21: fade = 1; break;					//red fade
		case 22: fade = 2; break;					//green fade
		case 23: fade = 3; break;					//blue fade
		case 24: fade = 4; break;					//white fade
		default: break;
	}
	return fade;
}



int main( void ) {
	
	USARTInitSTDIO(3);									//prepare Serial3 of uC for standard-I/O   
	USARTInit(3, 9600, 1, 0, 1, 0);						//set communication-parameters for Serial3
	
	TCCR1A = (1<<WGM00) | (1<<WGM01) | (1<<COM0A1);		//Timer 1 for Pin 11
	TCCR1B = (1<<CS01);
	
	TCCR3A = (1<<WGM00) | (1<<WGM01) | (1<<COM0A1);		//Timer 3 for Pin 5
	TCCR3B = (1<<CS01);
	
	TCCR4A = (1<<WGM00) | (1<<WGM01) | (1<<COM0A1);		//Timer 4 for Pin 6
	TCCR4B = (1<<CS01);
	
	sei();												//activate global interrupt mask
	DDRB = 0xFF;
	DDRE = 0xFF;
	DDRH = 0xFF;
	
	char enterd;
	int enter = 0;
	int counter = 0;
	int fade = 0;
	int dir = 0;
	int jumper = 1;
	int colorJump = 0;
	int del = 5;
	
	printf("\nPlease enter:");								//output for console testing

	while(1) {
		
		if(counter > 255) {									//changes the direction of the fade counter
			dir = 1;
		}
		else if(counter == 0) {
			dir = 0;
		}
		
		if(USARTCharReceived(3)) {							//tests if a char is received
			scanf("%c", &enterd);
			printf("Test1:%c", enterd);						//Test Output
			
			enter = converter(enterd);						//sends the chars to convert to int
			printf("Test2:%d", enter);						//Test Output
			
						
			/*Selection Reset*/
			if(enter != (21 || 22 || 23 || 24)) {
				fade = 0;
			}
			if(enter != 25) {
				colorJump = 0;
			}
			if(enter == 25) {								//switches the delay to 300ms when color jumping mode is enabled
				del = 300;
			}
			else del = 5;
			
			/*Mode Selection*/
			if(enter <= 20) {								//single Color Mode when entry is below 20
				singeColor(enter);							
			}
			else if(enter > 20 && enter <= 24) {			//fade Mode when entry is from 21 to 24
				fade = fadeColor(enter);
			}
			else if(enter == 25) {							//jumpColor Mode when entry is 25
				colorJump = 1;
			}			
		}
		
		/*ColorFade*/
		switch(fade) {
			
			case 1: color(counter, 0, 0); break;				//red Fade
			case 2: color(0, counter, 0); break;				//green Fade
			case 3: color(0, 0, counter); break;				//blue Fade
			case 4: color(counter, counter, counter); break;	//white Fade
			default: break;
		}
		
		/*ColorJump*/
		if(colorJump == 1) {								//jumps between the 20 saved colors
			if(jumper > 20) {
				jumper = 1;
			}
			singeColor(jumper);
			jumper++;
		}
				
		if(dir == 0) {										//changes counter depending on direction
			counter++;
		}
		else counter--;

		delay(del);
	}
	return 0;
}