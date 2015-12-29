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

void setup_pinchange() {
  GIMSK |= (1<< PCIE);
  PCMSK |= (1 << PCINT0);
}

// IO ROUTINES
void flash() {
  PORTB |= (1 << PB3) | (1 << PB4);
  _delay_ms(2);
  PORTB &= ~((1 << PB3) | (1 << PB4));
}

volatile bool fast = false;
void timer_expired() {
  static unsigned c = 0;

  c += fast ? 23 : 1;

  if (c >= 2 * 42) {
    flash();
    c = 0;
  }
}

// INTERRUPT ROUTINES
ISR(WDT_vect) {
  timer_expired();
}

ISR(PCINT0_vect) {
  fast = PINB & (1 << PB0);
}

void save_power_prepare() {
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
  setup_pinchange();
  save_power_prepare();
  sei();

  while (1)
    save_power_do();

  return 0;
}

