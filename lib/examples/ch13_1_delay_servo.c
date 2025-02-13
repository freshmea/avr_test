

// 딜레이로 pwm 구현시 오작동이 심하다.
#include <avr/io.h>     //AVR 입출력에 대한 헤더 파일
#include <util/delay.h> //delay 함수사용을 위한 헤더파일

int main(void)
{
    DDRE = 0x08; // SERVO 핀을 출력 포트로 설정 한다.

    while (1)
    { // PWM 주기를 20[ms]로 한다.
        PORTE |= 0x08;
        _delay_us(1500); // ON Time이 1.5[ms]
        PORTE &= ~0x08;
        _delay_us(18500); // OFF Time이 18.5[ms]
    }
}
