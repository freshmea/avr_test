
#include <avr/io.h>     //AVR 입출력에 대한 헤더 파일
#include <util/delay.h> //delay 함수사용을 위한 헤더파일

#define M1_Forword 0x10
#define M1_Reverse 0x20
#define M1_Enable 0x20

int main(void)
{
    DDRD = 0x30; // MOTOR1_IN1, MOTOR1_IN2를 출력 포트로 설정 한다.
    DDRB = 0x20; // MOTOR1_EN 를 출력 포트로 설정 한다.

    TCCR1A = 0x82; // COMnA1=1 COMnA0=0 COMnB1=0 COMnB0=0 COMnC1=0 COMnC0=0 WGMn1=1 WGMn0=0
    TCCR1B = 0x19; // ICNCn=0 ICESn=0 Blink=0 WGMn3=1 WGMn2=1 CSn2=0 CSn1=0 CSn0=1
    TCCR1C = 0x00;

    ICR1 = 800;  // 14745600/18432 => 800
    OCR1A = 560; // 듀티비 70%

    while (1)
    {
        // DC Motor 정회전
        PORTD = M1_Forword;
        _delay_ms(1000);
        // DC Motor 역회전
        PORTD = M1_Reverse;
        _delay_ms(1000);
    }
}
