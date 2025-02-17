#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRC = 0x0F; // 포트C를 출력포트로 설정

    while (1)
    {
        PORTC = 0x00;    // 모든 LED 끄기
        _delay_ms(1000); // 1초 대기
        PORTC = 0x0F;    // 모든 LED 켜기
        // PORTC = 0b00001111; // 모든 LED 켜기
        // PORTC = (1 << PA0) | (1 << PB1) | (1 << PC2) | (1 << PD3);
        // PORTC = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
        // PORTC = _BV(0) | _BV(1) | _BV(2) | _BV(3);
        // PORTC = _BV(PC0) | _BV(PC1) | _BV(PC2) | _BV(PC3);
        // PORTC = _BV(0);

        _delay_ms(1000); // 1초 대기
    }
    return 0;
}