
#include <avr/io.h>     //AVR 입출력에 대한 헤더 파일
#include <util/delay.h> //delay 함수사용을 위한 헤더파일

int main(void)
{
    unsigned char i;

    DDRB = 0x20; // MOTOR1_EN 를 출력 포트로 설정 한다.
    DDRD = 0xF0; // STEP0, STEP1, STEP2, STEP3을 출력 포트로 설정 한다.

    PORTB &= ~0x20; // M1 Disable, DC 모터 정지

    while (1)
    {
        for (i = 0; i < 12; i++) // 48 스텝 실행
        {
            PORTD = 0x30; // 1 step
            _delay_ms(10);
            PORTD = 0x90; // 2 step
            _delay_ms(10);
            PORTD = 0xC0; // 3 step
            _delay_ms(10);
            PORTD = 0x60; // 4 step
            _delay_ms(10);
        }
        _delay_ms(1000);
    }
}
