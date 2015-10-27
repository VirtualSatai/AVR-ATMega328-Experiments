#include <avr/io.h>
#include <stdint.h>            // has to be added to use uint8_t

#include <avr/interrupt.h>    // Needed to use interrupts    

#define F_CPU 16000000UL // 8 MHz
#include <util/delay.h>

volatile uint8_t portbhistory = 0xFF;     // default is high because the pull-up


int main(void)
{
    DDRB &= ~((1 << DDB0) | (1 << DDB1) | (1 << DDB2)); // Clear the PB0, PB1, PB2 pin
    // PB0,PB1,PB2 (PCINT0, PCINT1, PCINT2 pin) are now inputs

    PORTB |= ((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2)); // turn On the Pull-up
    // PB0, PB1 and PB2 are now inputs with pull-up enabled
    
    PCICR |= (1 << PCIE0);     // set PCIE0 to enable PCMSK0 scan
    PCMSK0 |= ((1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2));   // set PCINT0 to trigger an interrupt on state change 

    
    DDRD |= ((1 << DDD5) | (1 << DDD6) | (1 << DDD7));  // Set output pins digital 5 - 7
    PORTD |= ((1 << DDD5) | (1 << DDD6) | (1 << DDD7)); // Turn digital 5-7 on

    sei();                     // turn on interrupts

    while(1)
    {
        _delay_ms(1000);
        PORTD &= ~((1 << DDD5) | (1 << DDD6) | (1 << DDD7)); // Turn digital 5-7 off
    }
}



ISR (PCINT0_vect)
{
    uint8_t changedbits;

    changedbits = PINB ^ portbhistory;
    portbhistory = PINB;
    
    if(changedbits & (1 << PINB0))
    {
        PORTD |= ((1 << DDD5));
    }
    
    if(changedbits & (1 << PINB1))
    {
        PORTD |= (1 << DDD6);
    }

    if(changedbits & (1 << PINB2))
    {
        PORTD |= (1 << DDD7);
    }
    
} 