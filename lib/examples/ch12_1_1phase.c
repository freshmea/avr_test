
#include <avr/io.h>     //AVR 입출력에 대한 헤더 파일
#include <util/delay.h> //delay 함수사용을 위한 헤더파일

int main(void)
{
    DDRB = 0x20; // MOTOR1_EN 를 출력 포트로 설정 한다.
    DDRD = 0xF0; // STEP0, STEP1, STEP2, STEP3을 출력 포트로 설정 한다.

    PORTB &= ~0x20; // M1 Disable, DC 모터 정지

    while (1)
    {
        PORTD = 0x10; // 1 step
        _delay_ms(10);
        PORTD = 0x20; // 2 step
        _delay_ms(10);
        PORTD = 0x40; // 3 step
        _delay_ms(10);
        PORTD = 0x80; // 4 step,
        _delay_ms(10);
    }
}
