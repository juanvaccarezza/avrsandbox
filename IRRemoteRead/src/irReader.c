/* Raw IR commander

 This sketch/program uses the Arduno and a PNA4602 to
 decode IR received.  It then attempts to match it to a previously
 recorded IR signal

 Code is public domain, check out www.ladyada.net and adafruit.com
 for more tutorials!
 */

// We need to use the 'raw' pin reading methods
// because timing is very important here and the digitalRead()
// procedure is slower!
//uint8_t IRpin = 2;
// Digital pin #2 is the same as Pin D2 see
// http://arduino.cc/en/Hacking/PinMapping168 for the 'raw' pin mapping

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <math.h>

#define IRpin_PIN      PINC
#define IRpin          2

// the maximum pulse we'll listen for - 65 milliseconds is a long time
#define MAXPULSE 65000
#define NUMPULSES 50

// what our timing resolution should be, larger is better
// as its more 'precise' - but too large and you wont get
// accurate timing
#define RESOLUTION 20

// What percent we will allow in variation to match the same code
#define FUZZINESS 20

// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[NUMPULSES][2];  // pair is high and low pulse
uint8_t currentpulse = 0; // index for pulses we're storing

#include "ircodes.h"
#include "irReader.h"

bool IRcompare(int numpulses, int Signal[], int refsize);
int listenForIR(void);

void loop(void) {
  int numberpulses;

  numberpulses = listenForIR();

  //printf("Heard ");
  printf("%d",numberpulses);
  //printf("-pulse long IR signal\n");
  if (IRcompare(numberpulses, OneIRsignal,sizeof(OneIRsignal)/4)) {
    printf("Unooooooooooo\n");
  } else if (IRcompare(numberpulses, TwoIRsignal,sizeof(TwoIRsignal)/4)) {
    printf("Dossss\n");
  } else {
    printf("Caca\n");
  }
  _delay_ms(500);
}

//KGO: added size of compare sample. Only compare the minimum of the two
bool IRcompare(int numpulses, int Signal[], int refsize) {
  int count = numpulses<=refsize?numpulses:refsize;
  printf("count set to: ");
  printf("%d\n",count);
  for (int i=0; i< count-1; i++) {
    int oncode = pulses[i][1] * RESOLUTION / 10;
    int offcode = pulses[i+1][0] * RESOLUTION / 10;

#ifdef DEBUG
    printf(oncode); // the ON signal we heard
    printf(" - ");
    printf(Signal[i*2 + 0]); // the ON signal we want
#endif

    // check to make sure the error is less than FUZZINESS percent
    if ( abs(oncode - Signal[i*2 + 0]) <= (Signal[i*2 + 0] * FUZZINESS / 100)) {
#ifdef DEBUG
      printf(" (ok)");
#endif
    } else {
#ifdef DEBUG
      printf(" (x)");
#endif
      // we didn't match perfectly, return a false match
      return false;
    }


#ifdef DEBUG
    printf("  \t"); // tab
    printf(offcode); // the OFF signal we heard
    printf(" - ");
    printf(Signal[i*2 + 1]); // the OFF signal we want
#endif

    if ( abs(offcode - Signal[i*2 + 1]) <= (Signal[i*2 + 1] * FUZZINESS / 100)) {
#ifdef DEBUG
      printf(" (ok)");
#endif
    } else {
#ifdef DEBUG
      printf(" (x)");
#endif
      // we didn't match perfectly, return a false match
      return false;
    }

#ifdef DEBUG
    printfln();
#endif
  }
  // Everything matched!
  return true;
}

int listenForIR(void) {
  currentpulse = 0;

  while (1) {
    uint16_t highpulse, lowpulse;  // temporary storage timing
    highpulse = lowpulse = 0; // start out with no pulse length

    while (IRpin_PIN & (1 << IRpin)) {
       // pin is still HIGH

       // count off another few microseconds
       highpulse++;
       _delay_us(RESOLUTION);

       // If the pulse is too long, we 'timed out' - either nothing
       // was received or the code is finished, so print what
       // we've grabbed so far, and then reset

       // KGO: Added check for end of receive buffer
       if (((highpulse >= MAXPULSE) && (currentpulse != 0))|| currentpulse == NUMPULSES) {
         return currentpulse;
       }
    }
    // we didn't time out so lets stash the reading
    pulses[currentpulse][0] = highpulse;

    // same as above
    while (! (IRpin_PIN & _BV(IRpin))) {
       // pin is still LOW
       lowpulse++;
       _delay_us(RESOLUTION);
        // KGO: Added check for end of receive buffer
        if (((lowpulse >= MAXPULSE)  && (currentpulse != 0))|| currentpulse == NUMPULSES) {
         return currentpulse;
       }
    }
    pulses[currentpulse][1] = lowpulse;

    // we read one high-low pulse successfully, continue!
    currentpulse++;
  }
  return 0;
}
void printpulses(void) {
  printf("\n\r\n\rReceived: \n\rOFF \tON\n");
  for (uint8_t i = 0; i < currentpulse; i++) {
    printf("%d",pulses[i][0] * RESOLUTION);
    printf(" usec, ");
    printf("%d",pulses[i][1] * RESOLUTION);
    printf(" usec\n");
  }

  // print it in a 'array' format
  printf("int IRsignal[] = {\n");
  printf("// ON, OFF (in 10's of microseconds)\n");
  for (uint8_t i = 0; i < currentpulse-1; i++) {
    printf("\t"); // tab
    printf("%d",pulses[i][1] * RESOLUTION / 10);
    printf(", ");
    printf("%d",pulses[i+1][0] * RESOLUTION / 10);
    printf(",\n");
  }
  printf("\t"); // tab
  printf("%d",pulses[currentpulse-1][1] * RESOLUTION / 10);
  printf(", 0};");
}
