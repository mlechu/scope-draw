#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__ 1
#endif

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#define u8 uint8_t
#define i8 int8_t
#define u16 uint16_t
#define i16 int16_t

#define B(b) (1 << (b))
#define MAX(a, b) (a > b ? a : b)
#define MIN(a, b) (a < b ? a : b)
#define DIF(a, b) (a > b ? a - b : b - a)

/* scope */
u8 B_0[][2] = {{123, 78}, {4, 78},    {4, 24},   {123, 24},
               {123, 78}, {102, 99}, {24, 99}, {4, 78}};
u8 S_0[][2] = {{42, 73}, {42, 33}, {88, 33}, {88, 73}, {42, 73}};
u8 B_1[][2] = {{85, 59}, {44, 59}, {44, 40}, {85, 40},
               {85, 59}, {78, 67}, {51, 67}, {44, 59}};
u8 S_1[][2] = {{57, 57}, {57, 43}, {73, 43}, {73, 57}, {57, 57}};
u8 B_2[][2] = {{71, 51}, {59, 51}, {59, 45}, {71, 45},
               {71, 51}, {69, 53}, {61, 53}, {59, 51}};
u8 S_2[][2] = {{62, 50}, {62, 46}, {68, 46}, {68, 50}, {62, 50}};

u8 initialized = 0;

inline void set_beam(u8 x, u8 y) {
    OCR1A = x; /* duty cycle PB1 (pin 9) */
    OCR1B = y; /* duty cycle PB2 (pin 10) */
}

void init() {
    initialized = 1;
    DDRB = B(DDB1) | B(DDB2); /* enable output pins */
    set_beam(0, 0);
    TCCR1A = B(COM1A1) | B(COM1B1); /* (assuming wgm=fast) clear up; set down */
    TCCR1B = B(CS10);               /* No clock prescaling */
    /* TCCR1A |= B(WGM10), TCCR1B |= B(WGM12); /\* Fast PWM, 8-bit *\/ */
    TCCR1A |= B(WGM11), TCCR1B |= B(WGM12) | B(WGM13); /* fast PWM, ICR1a=top */
    ICR1 = 0x7f;
}

/* TODO: Currently handles nearly-straight lines well, but nearly-diagonal lines
 * poorly.  Distribute straight segments between diagonal ones if la/lb == 1. */
void draw_line(u8 x0, u8 y0, u8 x1, u8 y1) {
    /* a is the axis with more movement */
    /* max(lx,ly) = la is a decent approximation of the line length */
    /* more steps = brighter line */
    u8 axis = (DIF(x0, x1) > DIF(y0, y1));
    u8 a0, a1, b0, b1;
    if (axis) {
        a0 = x0, a1 = x1, b0 = y0, b1 = y1;
    } else {
        a0 = y0, a1 = y1, b0 = x0, b1 = x1;
    }
    u8 la = DIF(a0, a1);
    u8 lb = DIF(b0, b1);
    i8 sa = a0 < a1 ? 1 : -1;
    i8 sb = b0 < b1 ? 1 : -1;
    /* when b_i == 0, step b and reset b_i to b_s_per */
    u8 b_s_per = lb == 0 ? 255 : ((la / lb) - 1);
    /* don't update b after b_end steps */
    u8 b_end = (1 + b_s_per) * lb;

    u8 a = a0;
    u8 b = b0;
    set_beam((axis ? a : b), (axis ? b : a));
    u8 i = 0;
    u8 b_i = b_s_per;
    do {
        a += sa;
        if (i < b_end && b_i == 0) {
            b_i = b_s_per;
            b += sb;
        } else {
            b_i--;
        }
        set_beam((axis ? a : b), (axis ? b : a));
        _delay_us(100);
    } while (++i != 0 && i < la);
}

void draw(u8 (*p)[2], u8 n) {
    for (u8 i = 0; i < n - 1; i++) {
        draw_line(p[i][0], p[i][1], p[i + 1][0], p[i + 1][1]);
    }
}

int main() {
    init();
    while (1) {
        draw(B_0, 8);
        draw(S_0, 5);
        draw(B_1, 8);
        draw(S_1, 5);
        draw(B_2, 8);
        draw(S_2, 5);
    }
}
