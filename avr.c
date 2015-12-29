#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
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

volatile bool fast = false;

void toggle_led() {
  static unsigned c = 0;

  if (fast)
    c += 23;
  else
    c++;

  if (c >= 2 * 42) {
    flash();
    c = 0;
  }
}

// INTERRUPT ROUTINES
ISR(TIM0_COMPA_vect) {
  // toggle_led();
}

ISR(WDT_vect) {
  toggle_led();
}

ISR(PCINT0_vect) {
  const bool antenna = PINB & (1 << PB0);

  if (antenna) {
    // TCCR0B = (1 << CS01);
    fast = true;
  } else {
    // TCCR0B = (1 << CS02);
    fast = false;
  }
}

void save_power_ready() {
  PRR |= 11; // PRTIM1 + PRUSI + PRADC
  WDTCR |= (1 << WDIE);
  WDTCR &= ~((1 << WDP0) | (1 << WDP1) | (1 << WDP2) | (1 << WDP3));
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void save_power_do() {
  sleep_mode();
}

// MAIN PROGRAM
int main() {
  setup_port();
  // setup_timer();
  setup_pinchange();
  save_power_ready();
  sei();

  while (1)
    save_power_do();

  return 0;
}

