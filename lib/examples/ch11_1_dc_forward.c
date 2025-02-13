
#include <avr/io.h>     //AVR 입출력에 대한 헤더 파일
#include <util/delay.h> //delay 함수사용을 위한 헤더파일

#define M1_Forword 0x10
#define M1_Reverse 0x20
#define M1_Enable 0x20

int main(void)
{
    DDRD = 0x30; // MOTOR1_IN1, MOTOR1_IN2를 출력 포트로 설정 한다.
    DDRB = 0x20; // MOTOR1_EN 를 출력 포트로 설정 한다.

    while (1)
    {
        // DC Motor 정회전
        PORTD = M1_Forword;
        PORTB = M1_Enable;
        _delay_ms(1000);
        // DC Motor 역회전
        PORTD = M1_Reverse;
        PORTB = M1_Enable;
        _delay_ms(1000);
    }
}
