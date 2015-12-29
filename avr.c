#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// SETUP ROUTINES
void setup_port() {
  PORTB = 0x00;

  DDRB = (1 << PB3) | (1 << PB4);
}

void setup_timer() {
  TCCR0A |= (1 << COM0A0);
  TCCR0B = (1 << CS02);
  TIMSK |= (1 << OCIE0A); // Timer overflow interrupt enable - Note: also can do output compare match.
  sei();
}

void setup_pinchange() {
  GIMSK |= (1<< PCIE);
  PCMSK |= (1 << PCINT0);
}

// IO ROUTINES
void flash() {
  static char state = 0;

  if (state++ % 2)
    PORTB |= (1 << PB3);
  else
    PORTB |= (1 << PB4);

  _delay_ms(2);

  PORTB &= ~((1 << PB3) | (1 << PB4));
}

void toggle_led() {
  static unsigned c = 0;

  if (c++ == 88)
    c = 0;

  if (c == 0 || c == 44)
    flash();
}

// INTERRUPT ROUTINES
ISR(TIM0_COMPA_vect) {
  toggle_led();
}

ISR(PCINT0_vect) {
  const bool antenna = PINB & (1 << PB0);

  if (antenna) {
    TCCR0B = (1 << CS01);
  } else {
    TCCR0B = (1 << CS02);
  }
}

void save_power() {
  PRR |= 11; // PRTIM1 + PRUSI + PRADC
}

// MAIN PROGRAM
int main() {
  setup_port();
  setup_timer();
  setup_pinchange();
  save_power();

  while (1)
    asm volatile("nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;");

  return 0;
}

