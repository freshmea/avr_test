
#include "lcd.h"        //Text LCD를 사용하기 위한 헤더 파일
#include <avr/io.h>     //AVR 입출력에 대한 헤더 파일
#include <util/delay.h> //delay 함수사용을 위한 헤더파일

#define M1_Forword 0x10
#define M1_Reverse 0x20
#define M1_Enable 0x20

int main(void)
{
    unsigned char pwmduty = 60, cnt_dir = 0;

    DDRD = 0x30; // MOTOR1_IN1, MOTOR1_IN2를 출력 포트로 설정 한다.
    DDRB = 0x20; // MOTOR1_EN 를 출력 포트로 설정 한다.

    TCCR1A = 0x82; // COMnA1=1 COMnA0=0 COMnB1=0 COMnB0=0 COMnC1=0 COMnC0=0 WGMn1=1 WGMn0=0
    TCCR1B = 0x19; // ICNCn=0 ICESn=0 Blink=0 WGMn3=1 WGMn2=1 CSn2=0 CSn1=0 CSn0=1
    TCCR1C = 0x00;

    ICR1 = 800;  // 14745600/18432 => 800
    OCR1A = 560; // 듀티비 70%

    PORTD = M1_Forword; // DC Motor 정회전

    lcdInit();                  // Text LCD를 초기화
    lcdGotoXY(0, 0);            // 현재 커서위치를 첫번째줄 첫번째칸으로 이동한다.
    lcdPrintData(" Duty: ", 7); // " Duty: " 출력

    while (1)
    {
        if (cnt_dir)
        {
            pwmduty = pwmduty - 5; // pwmduty가 1씩 감소한다
            if (pwmduty < 50)
                cnt_dir = 0; // pwmduty가 0이 되면 pwmduty가 증가하기 시작한다.
        }
        else
        {
            pwmduty = pwmduty + 5; // pwmduty가 1씩 증가한다
            if (pwmduty > 99)
                cnt_dir = 1; // pwmduty가 100이 되면 pwmduty가 감소하기 시작한다.
        }
        OCR1A = 8 * pwmduty; // 듀티비가 1%씩 증가한다
        lcdGotoXY(7, 0);
        lcdDataWrite((pwmduty / 10) % 10 + '0'); // 10의 자리 출력
        lcdDataWrite((pwmduty) % 10 + '0');      // 1의 자리 출력
        lcdDataWrite('%');                       //%출력
        _delay_ms(1000);
    }
}
