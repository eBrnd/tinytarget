#include <stdio.h>
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
  TCCR0B |= (1 << CS02);
  TIMSK |= (1 << OCIE0A); // Timer overflow interrupt enable - Note: also can do output compare match.
  sei();
}

// IO ROUTINES
void toggle_led() {
  static unsigned c = 0;

  if (c++ == 88)
    c = 0;

  if (c == 0)
    PORTB |= (1 << PB3);
  else if (c == 1)
    PORTB &= ~((1 << PB4) | (1 << PB3));
  else if (c == 44)
    PORTB |= (1 << PB4);
  else if (c == 45)
    PORTB &= ~((1 << PB4) | (1 << PB3));
}

// INTERRUPT ROUTINES
ISR(TIM0_COMPA_vect) {
  toggle_led();
}

void save_power() {
  PRR |= 11; // PRTIM1 + PRUSI + PRADC
}

// MAIN PROGRAM
int main() {
  setup_port();
  setup_timer();
  save_power();

  while (1)
    asm volatile("nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;");

  return 0;
}

