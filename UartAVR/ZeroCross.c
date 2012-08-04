#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
// AVR306: Using the AVR UART in C
// Routines for interrupt controlled USART
// Last modified: 02-06-21
// Modified by: AR

// AVR306: Using the AVR UART in C
// Routines for polled UART
// Last modified: 02-06-21
// Modified by: AR

/* Includes */

/* Prototypes */
unsigned char ReceiveByte(void);
void TransmitByte(unsigned char data);

#define FOSC 2000000// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

void USART_Init(unsigned int ubrr) {
	/* Set baud rate */
	UBRRH = (unsigned char) (ubrr >> 8);
	UBRRL = (unsigned char) ubrr;
	/* Enable receiver and transmitter */UCSRB = (1 << RXEN) | (1 << TXEN);
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1 << URSEL) | (1 << USBS) | (3 << UCSZ0);
}

/* Main - a simple test program*/
int main(void) {
	DDRD = 0b00000010;
	DDRC = _BV(PC0); /* PC0 is digital output (alive signal) */
	USART_Init ( MYUBRR );
	for (;;) /* Forever */
	{
		TransmitByte('a');
		_delay_ms(1000);
		PORTC = ~PORTC;

	}
	return 0;
}



/* Read and write functions */
unsigned char ReceiveByte(void) {
	while (!(UCSRA & (1 << RXC)))
		/* Wait for incomming data */
		; /* Return the data */
	return UDR ;
}

void TransmitByte(unsigned char data) {
	while (!(UCSRA & (1 << UDRE)))
		; /* Wait for empty transmit buffer */
	UDR = data; /* Start transmittion */
}
