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

#define FOSC 12000000
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

		sbi(PORTC, STATUS_LED);
		_delay_ms(1000);

		cbi(PORTC, STATUS_LED);
		_delay_ms(1000);
    }

    return(0);
}

void ioinit (void)
{
	sbi(DDRC,DDC0);
	/* Set baud rate */
	UBRR0H = (unsigned char)(MYUBRR>>8);
	UBRR0L = (unsigned char)MYUBRR;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	stdout = &mystdout;

}


static int uart_putchar(char c, FILE *stream)
{
    if (c == '\n') uart_putchar('\r', stream);

    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;

    return 0;
}

uint8_t uart_getchar(void)
{
    while( !(UCSR0A & (1<<RXC0)) );
    return(UDR0);
}

