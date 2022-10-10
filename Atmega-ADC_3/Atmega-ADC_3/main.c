/*
 * Atmega-ADC_3.c
 *
 * Created: 08.12.2021 1:14:29
 * Author : tvoty
 */ 

#define F_CPU 11059200
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTB0
#define EN eS_PORTB1
#include "lcd.h"

volatile int tick=0;

int main(void)
{
	DDRD = 0xFF; DDRB = 0xFF;
	sei();
    Lcd4_Init();
	ADCSRA |= 1<<ADEN ; // AD enable
	ADCSRA |= 0x07 ;	// prescaler 128
	ADMUX |= 1<<REFS0;	// set ext AD ref
	
//	ADCSRA |=  1<<ADIE;	// AD interrupt enable

	TCCR1B |= 1<<CS12 | 1<<WGM12;	// prescaler 256 | Clear timer 1 on compare match
	TIMSK |= 1<<OCIE1A;				// compare match interrupt enable
	OCR1A = 43200/10;				// 43200 = 1 sec <- 11M0592
	ADCSRA |= 1<<ADATE;				// auto/trigger enable
	SFIOR |= 0xA0;					// 0x0_ = free-run, 0xA_ = timer 1 compare match
	
	ADCSRA |= 1<<ADSC;	// start AD 
	
	//float freq, i=0;
	//char string[5];
	Lcd4_Clear();
    while (1) 
    {
		/*if(tick==1)
		{
			freq = 60/((i*1)/1000);
			itoa(freq, string, 10);
			Lcd4_Set_Cursor(1,0);	Lcd4_Write_String(string);	Lcd4_Write_String(" ");
			i=0;
			tick =0;
		}
		_delay_ms(1);
		i++;*/
    }
}

// FREERUNNIG -> interrupt zavolán OKAMŽITÌ po tom, co dobìhne AD pøevod
ISR(ADC_vect)
{
	char string[5];
	uint16_t num;
	num = ADCL | (ADCH << 8);

	itoa(num, string, 10 );
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String(string);
	Lcd4_Write_String(" ");
/*
	if( (ADCL | (ADCH << 8)) >500)
		tick = 1;
*/			
	ADCSRA |= 1<<ADSC; 
}

// ÈASOVACÍ REŽIM -> interrupt je zavolán až po tom, co DOBÌHNE ÈASOVAÈ
ISR(TIMER1_COMPA_vect)
{
	char string[5];
	uint16_t num;
	num = ADCL | (ADCH << 8);

	itoa(num, string, 10 );
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String(string); Lcd4_Write_String(" ");

	ADCSRA |= 1<<ADSC; 
}

