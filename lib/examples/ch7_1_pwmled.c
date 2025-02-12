#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    // PB4 핀을 출력으로 설정 (비트 PB4를 1로 설정)
    DDRB |= (1 << PB4);

    // Timer0 Fast PWM 모드, 비반전 모드, 분주비 8
    // WGM00 = 1, WGM01 = 1 : Fast PWM 모드
    // COM01 = 1, COM00 = 0 : 비반전 모드 구동 (PWM 출력이 OC0 핀에 출력됨)
    // CS01 = 1 : 분주비 8 (CS00, CS02는 0)
    TCCR0 = (1 << WGM00) | (1 << WGM01) | (1 << COM01) | (1 << CS01);
    // OCR0의 값에 따라 PWM 듀티 비율이 조정됨

    uint8_t brightness = 0;
    int8_t delta = 1; // 밝기 증감량

    while (1)
    {
        OCR0 = brightness;
        _delay_ms(10);

        brightness += delta;
        if (brightness == 0 || brightness == 255)
        {
            delta = -delta;
        }
    }
    return 0; // 도달하지 않음
}