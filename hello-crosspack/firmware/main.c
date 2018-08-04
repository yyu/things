#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB |= (1 << PB5);

    for(;;){
        PORTB = 0b00100000; /* Turn on first LED bit/pin in PORTB */
        _delay_ms(1000);    /* wait */

        PORTB = 0b00000000; /* Turn off all B pins, including LED */
        _delay_ms(1000);    /* wait */
    }

    return 0;               /* never reached */
}
