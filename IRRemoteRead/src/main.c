/*
    5-10-07
    Copyright Spark Fun Electronics© 2007
    Nathan Seidle
    nathan at sparkfun.com

    Example basic printf input/output
*/

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "irReader.h"

#define FOSC 2000000
#define BAUD 9600
#define MYUBRR (FOSC/16/BAUD-1)

#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask))

#define STATUS_LED 0


//Define functions
//======================
void ioinit(void);      // initializes IO
static int uart_putchar(char c, FILE *stream);
uint8_t uart_getchar(void);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);



int main (void)
{
	uint8_t x = 0;

    ioinit(); //Setup IO pins and defaults

    while(1)
    {
		x++;
		printf("Test it! x = %d\n", x);
		loop();
		//printpulses();
		sbi(PORTC, STATUS_LED);
		_delay_ms(500);

		cbi(PORTC, STATUS_LED);
		_delay_ms(500);
    }

    return(0);
}

void ioinit (void)
{
    //1 = output, 0 = input
    DDRB = 0b11101111; //PB4 = MISO
    DDRC = 0b11111011; //
    DDRD = 0b11111110; //PORTD (RX on PD0)

    //USART Baud rate: 9600
    UBRRH = MYUBRR >> 8;
    UBRRL = MYUBRR;
    UCSRB = (1<<RXEN)|(1<<TXEN);
    /* Set frame format: 8data, 2stop bit */
    UCSRC = (1 << URSEL) | (1 << USBS) | (3 << UCSZ0);
    stdout = &mystdout; //Required for printf init
}


static int uart_putchar(char c, FILE *stream)
{
    if (c == '\n') uart_putchar('\r', stream);

    loop_until_bit_is_set(UCSRA, UDRE);
    UDR = c;

    return 0;
}

uint8_t uart_getchar(void)
{
    while( !(UCSRA & (1<<RXC)) );
    return(UDR);
}




