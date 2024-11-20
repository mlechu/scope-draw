#define __AVR_ATmega328P__ 1
#include <avr/io.h>
/* #include <avr/iom328p.h> */
/* #include <avr/wdt.h> */
/* #include <avr/sleep.h> */

#define B(b) (1 << (b))

/* scope */
int B_0[][2] = {{123, 78}, {4, 78},    {4, 24},   {123, 24},
                {123, 78}, {102, 110}, {24, 110}, {4, 78}};
int S_0[][2] = {{42, 73}, {42, 33}, {88, 33}, {88, 73}, {42, 73}};
int B_1[][2] = {{85, 59}, {44, 59}, {44, 40}, {85, 40},
                {85, 59}, {78, 69}, {51, 69}, {44, 59}};
int S_1[][2] = {{57, 57}, {57, 43}, {73, 43}, {73, 57}, {57, 57}};
int B_2[][2] = {{71, 51}, {59, 51}, {59, 45}, {71, 45},
                {71, 51}, {69, 54}, {61, 54}, {59, 51}};
int S_2[][2] = {{63, 50}, {63, 46}, {68, 46}, {68, 50}, {63, 50}};

int initialized = 0;

void init() {
    initialized = 1;
    DDRB = B(DDB1) | B(DDB2); /* enable output pins */

    OCR1A = 250; /* duty cycle PB1 (pin 9) */
    OCR1B = 128; /* duty cycle PB2 (pin 10) */

    TCCR1A = B(COM1A1) | B(COM1B1); /* (assuming wgm=fast) clear up; set down */
    TCCR1B = B(CS10);               /* No clock prescaling */
    TCCR1A |= B(WGM10), TCCR1B |= B(WGM12); /* Fast PWM, 8-bit */
}

int main() {
    init();
    while (1);
}
