/*
 * Mini_Project2.c
 * By: Amr Mohamed Yousef
 * Diploma Number: 83
 * Facebook Name: Amr Mohamed
 * ======================================================================================================================================
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define F_CPU 1000000UL
#define DELAY 2

unsigned char SEC1 = 0, SEC2 = 0, MIN1 = 0, MIN2 = 0, HRS1 = 0, HRS2 = 0, inc_flag = 0;

void increment(void); /* function prototype */

ISR(TIMER1_COMPA_vect)
{
	inc_flag = 1;
}

ISR(INT0_vect)
{
	HRS2 = 0;
	HRS1 = 0;
	MIN2 = 0;
	MIN1 = 0;
	SEC1 = 0;
	SEC2 = 0;
}

ISR(INT1_vect)
{
	TCCR1B &= 0xF8; /* No clock source to pause the timer*/
}

ISR(INT2_vect)
{
	TCCR1B |= (1<<CS10) | (1<<CS12);
}

void Timer1_INT(void)
{
	TCNT1 = 0;
	OCR1A = 975;
	TIMSK |= (1<<OCIE1A);
	TCCR1A = (1<<FOC1A);
	TCCR1B = (1<<WGM12) | (1<<CS10) | (1<<CS12);
}

void EXT_INT0(void)
{
	DDRD &= ~(1<<PD2); /* setting pin 2 in PORTD to input */
	PORTD |= (1<<PD2); /* enabling pull up resistor for pin 2 in PORTD */
	/* setting INT0 to falling edge */
	MCUCR |= (1<<ISC01);
	MCUCR &= ~(1<<ISC00);
	GICR |= (1<< INT0); /* External Interrupt Request 0 Enable */
}

void EXT_INT1(void)
{
	DDRD &= ~(1<<PD3); /* setting pin 3 in PORTD to input */
	/* setting INT1 to falling edge */
	MCUCR |= (1<<ISC10) | (1<<ISC11);
	GICR |= (1<< INT1); /* External Interrupt Request 1 Enable */
}

void EXT_INT2(void)
{
	DDRB &= ~(1<<PB2); /* setting pin 2 in PORTB to input */
	PORTB |= (1<<PB2); /* enabling pull up resistor for pin 2 in PORTB */
	/* setting INT2 to falling edge */
	MCUCSR &= ~(1<<ISC2);
	GICR |= (1<< INT2); /* External Interrupt Request 2 Enable */
}

int main(void)
{
	sei(); /* setting I-bit in SREG to enable */
	DDRC |= 0x0F; /* setting the first four pins in PORTC to output */
	PORTC &= 0xF0; /* setting the initial value to the decoder to 0 */
	DDRA |= 0x3F; /* setting the first six pins in PORTA to output */
	Timer1_INT();
	EXT_INT0();
	EXT_INT1();
	EXT_INT2();
	while(1)
	{
		/*if(!(PIND & (1<<PD2)))
		{
			EXT_INT0();
		}
		if(PIND & (1<<PD3))
		{
			EXT_INT1();
		}
		if(!(PINB & (1<<PB2)))
		{
			EXT_INT2();
		}*/
		PORTA = (PORTA & 0xC0) | 0x01; /* Enabling the first 7-segment - SEC1 */
		PORTC = (PORTC & 0xF0) | (SEC1 & 0x0F);
		_delay_ms(DELAY);
		PORTA = (PORTA & 0xC0) | 0x02; /* Enabling the second 7-segment - SEC2 */
		PORTC = (PORTC & 0xF0) | (SEC2 & 0x0F);
		_delay_ms(DELAY);
		PORTA = (PORTA & 0xC0) | 0x04; /* Enabling the third 7-segment - MIN1 */
		PORTC = (PORTC & 0xF0) | (MIN1 & 0x0F);
		_delay_ms(DELAY);
		PORTA = (PORTA & 0xC0) | 0x08; /* Enabling the fourth 7-segment - MIN2 */
		PORTC = (PORTC & 0xF0) | (MIN2 & 0x0F);
		_delay_ms(DELAY);
		PORTA = (PORTA & 0xC0) | 0x10; /* Enabling the fifth 7-segment - HRS1 */
		PORTC = (PORTC & 0xF0) | (HRS1 & 0x0F);
		_delay_ms(DELAY);
		PORTA = (PORTA & 0xC0) | 0x20; /* Enabling the sixth 7-segment - HRS2 */
		PORTC = (PORTC & 0xF0) | (HRS2 & 0x0F);
		_delay_ms(DELAY);
		if(inc_flag == 1)
		{
			increment();
			inc_flag = 0;
		}
	}
}

void increment(void)
{
	SEC1++;
		if(SEC1 == 10)
		{
			SEC1 = 0;
			SEC2++;
			if(SEC2 == 6)
			{
				SEC2 = 0;
				SEC1 = 0;
				MIN1++;
				if(MIN1 == 10)
				{
					MIN1 = 0;
					MIN2++;
					if(MIN2 == 6)
					{
						MIN2 = 0;
						MIN1 = 0;
						HRS1++;
						if(HRS1 == 10)
						{
							HRS1 = 0;
							HRS2++;
							if((HRS2 == 2) && (HRS1 == 4))
							{
								HRS2 = 0;
								HRS1 = 0;
								MIN2 = 0;
								MIN1 = 0;
								SEC1 = 0;
								SEC2 = 0;
							}
						}
					}
				}
			}
		}
}
