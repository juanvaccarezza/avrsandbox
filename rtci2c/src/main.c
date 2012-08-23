/*
    5-10-07
    Copyright Spark Fun Electronics© 2007
    Nathan Seidle
    nathan at sparkfun.com

    Example basic printf input/output
*/

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "TWI_Master.h"

// Sample TWI transmission commands
#define TWI_CMD_MASTER_WRITE 0x10
#define TWI_SECONDS_ADDR  0x0




#define FOSC 2000000
#define BAUD 9600
#define MYUBRR (FOSC/16/BAUD-1)

#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask))

#define STATUS_LED 7

//Define functions
//======================
void ioinit(void);      // initializes IO
static int uart_putchar(char c, FILE *stream);
uint8_t uart_getchar(void);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

unsigned char TWI_Act_On_Failure_In_Last_Transmission ( unsigned char TWIerrorMsg )
{
                    // A failure has occurred, use TWIerrorMsg to determine the nature of the failure
                    // and take appropriate actions.
                    // Se header file for a list of possible failures messages.

                    // Here is a simple sample, where if received a NACK on the slave address,
                    // then a retransmission will be initiated.

  if ( (TWIerrorMsg == TWI_MTX_ADR_NACK) | (TWIerrorMsg == TWI_MRX_ADR_NACK) )
    TWI_Start_Transceiver();

  return TWIerrorMsg;
}

void waitForI2C() {
	while (TWI_Transceiver_Busy()) {
	}
	if(!TWI_statusReg.lastTransOK){
	       // Use TWI status information to detemine cause of failure and take appropriate actions.
	        TWI_Act_On_Failure_In_Last_Transmission( TWI_Get_State_Info( ) );
	        printf("Se destartalo todo\n");
	}
}

int main (void)
{
	uint8_t x = 0;
	unsigned char messageBuf[4];
	unsigned char TWI_targetSlaveAddress;

	TWI_targetSlaveAddress   = 0b01101111;

    ioinit(); //Setup IO pins and defaults
    TWI_Master_Initialise();
    sei();

    messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS) | (FALSE	<<TWI_READ_BIT); // The first byte must always consit of General Call code or the TWI slave address.
    messageBuf[1] = 0x07;             // The first byte is used for commands.
    messageBuf[2] = 0x10;             // The first byte is used for commands.
    TWI_Start_Transceiver_With_Data( messageBuf, 3 );

    waitForI2C();


//    messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS) | (FALSE	<<TWI_READ_BIT); // The first byte must always consit of General Call code or the TWI slave address.
//    messageBuf[1] = TWI_SECONDS_ADDR;             // The first byte is used for commands.
//    messageBuf[2] = 8;             // The first byte is used for commands.
//    TWI_Start_Transceiver_With_Data( messageBuf, 3 );
//
//    waitForI2C();


    while(1)
    {
		x++;

		printf("Test it! x = %d\n", x);

		sbi(PORTC, STATUS_LED);
		_delay_ms(1000);

		cbi(PORTC, STATUS_LED);
		_delay_ms(1000);


        messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS) | (FALSE	<<TWI_READ_BIT); // The first byte must always consit of General Call code or the TWI slave address.
        messageBuf[1] = TWI_SECONDS_ADDR;             // The first byte is used for commands.
        TWI_Start_Transceiver_With_Data( messageBuf, 2 );

        waitForI2C();

        messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS) | (TRUE	<<TWI_READ_BIT); // The first byte must always consit of General Call code or the TWI slave address.
        messageBuf[1] = 0;
        TWI_Start_Transceiver_With_Data( messageBuf, 4 );

        waitForI2C();

        TWI_Get_Data_From_Transceiver( messageBuf, 4 );

        printf("Data 0: %x\n",messageBuf[0]);
        printf("Data 1: %x\n",messageBuf[1]);

        printf("Tiempo(hh:mm:ss) > %x:%x:%x\n",messageBuf[3],messageBuf[2],messageBuf[1]);

        _delay_ms(1000);


    }

    return(0);
}

void ioinit (void)
{
	sbi(DDRC,DDC7);
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

