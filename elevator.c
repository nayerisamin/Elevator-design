#include <mega16a.h>

// A0: going up
// A1: going down

// weight sensor:
// B0: 0 weight
// B1: allowed weight
// B2: more than allowed weight

// elevator's location:
// B3: in floor 1
// B4: in floor 2
// B5: in floor 3

// inside buttons:
// C0: go to floor 1
// C1: go to floor 2
// C2: go to floor 3

// outside buttons:
// D0: floor 1 up button
// D1: floor 1 down button
// D2: floor 2 up button
// D3: floor 2 down button
// D4: floor 3 up button
// D5: floor 3 down button

const long int max_wait_count = 8000;
long int wait_counter = 0;

bit floor1 = 0;
bit floor2_up = 0;
bit floor2_down = 0;
bit floor3 = 0;

void main()
{
    DDRA = 0xFF;
    DDRB = DDRC = DDRD = 0x00;
    PORTA = 0x00;
    PORTB = 0b111111;
    PORTC = 0b111;
    PORTD = 0b111111;

    while (1)
    {
        // Input
        if (!(PINC & 0b001))
            floor1 = 1;
        if (!(PINC & 0b010))
        {
            floor2_up = 1;
            floor2_down = 1;
        }
        if (!(PINC & 0b100))
            floor3 = 1;

        if (!(PIND & 0b000001)) floor1 = 1;
        if (!(PIND & 0b000010)) floor1 = 1;
        if (!(PIND & 0b000100)) floor2_up = 1;
        if (!(PIND & 0b001000)) floor2_down = 1;
        if (!(PIND & 0b010000)) floor3 = 1;
        if (!(PIND & 0b100000)) floor3 = 1;

        if (~PINC & 0b111 || ~PIND & 0b111111) wait_counter = 0;

        // Stopping
        if (floor1 && !(PINB & 0b001000) && !(PORTA & 1)) // in floor 1 not going up
        {
            floor1 = 0;
            PORTA = 0;
        }
        else if (floor2_up && !(PINB & 0b010000) && !(PORTA & 2)) // in floor 2 not going down
        {
            floor2_up = 0;
            floor2_down = 0;
            PORTA = 0;
        }
        else if (floor2_down && !(PINB & 0b010000) && !(PORTA & 1)) // in floor 2 not going up
        {
            floor2_up = 0;
            floor2_down = 0;
            PORTA = 0;
        }
        else if (floor3 && !(PINB & 0b100000) && !(PORTA & 2)) // in floor 3 not going down
        {
            floor3 = 0;
            PORTA = 0;
        }

        // Starting
        if (!(PORTA) && (PINB & 0b101) && !(PINB & 0b010))
        {
            if (!(PINB & 0b001000) && (floor2_up || floor2_down || floor3))
            {
                if (wait_counter < max_wait_count) wait_counter++;
                else PORTA = 1;
            }
            else if (!(PINB & 0b010000) && (floor3))
            {
                if (wait_counter < max_wait_count) wait_counter++;
                else PORTA = 1;
            }
            else if (!(PINB & 0b010000) && (floor1))
            {
                if (wait_counter < max_wait_count) wait_counter++;
                else PORTA = 2;
            }
            else if (!(PINB & 0b100000) && (floor1 || floor2_up || floor2_down))
            {
                if (wait_counter < max_wait_count) wait_counter++;
                else PORTA = 2;
            }
        }
        else wait_counter = 0;
    }
}
