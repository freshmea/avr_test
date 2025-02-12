
#include <avr/interrupt.h> // AVR 인터럽트에 대한 헤더파일
#include <avr/io.h>        // AVR 입출력에 대한 헤더 파일

volatile unsigned char LED_Data = 0x01;
unsigned char timer2Cnt = 0, Shift_Flag = 0;

SIGNAL(TIMER2_OVF_vect); // Timer0 Overflow0 ISP

int main(void)
{
    DDRC = 0x0F; // 포트C 를 출력포트로 설정한다.

    TCCR2 = 0x05; // 프리스케일러 1024로 설정
    TCNT0 = 112;  // 256-144=112 -> 0.01초 마다 한번씩 인터럽트 발생
    TIMSK = 0x40; //
    TIFR |= 1 << TOV2;

    sei();

    while (1)
    {
        PORTC = LED_Data; // 포트C로 변수 LED_Data에 있는 데이타를 출력한다.
    }
}

SIGNAL(TIMER2_OVF_vect)
{
    cli();
    TCNT2 = 112; // 256-144=112 -> 0.01초 마다 한번씩 인터럽트 발생
    timer2Cnt++; // timer0Cnt 변수를 1 증가 시킨다.

    if (timer2Cnt == 50)
    { // 0.01s * 50 = 0.5s  //0.5초를 얻기 위한 카운트 횟수
        if (Shift_Flag == 0)
        {                         // LED0 ~ LED3을 이동
            LED_Data <<= 1;       // LED_Data 변수를 좌측으로 쉬프트한다.
            if (LED_Data == 0x08) // LED3으로 이동하면
                Shift_Flag = 1;   // 우측으로 쉬프트하도록 설정
        }
        else
        {                         // LED3 ~ LED0으로 이동
            LED_Data >>= 1;       // LED_Data 변수를 우측으로 쉬프트한다.
            if (LED_Data == 0x01) // LED0으로 이동하면
                Shift_Flag = 0;   // 좌측으로 쉬프트하도록 설정
        }
        timer2Cnt = 0;
    }
    sei();
}
