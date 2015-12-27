#include <stdbool.h>
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
  TCCR0B |= (1 << CS00) | (1 << CS02);
  TIMSK |= (1 << OCIE0A); // Timer overflow interrupt enable - Note: also can do output compare match.
  sei();
}

// IO ROUTINES
void toggle_led() {
  static unsigned c = 0;

  if (c++ % 2) {
    PORTB |= (1 << PB3);
    PORTB &= ~(1 << PB4);
  } else {
    PORTB &= ~(1 << PB3);
    PORTB |= (1 << PB4);
  }
}

// INTERRUPT ROUTINES
ISR(TIM0_COMPA_vect) {
  toggle_led();
}

// MAIN PROGRAM
int main() {
  setup_port();
  setup_timer();
  while(1);

  return 0;
}

