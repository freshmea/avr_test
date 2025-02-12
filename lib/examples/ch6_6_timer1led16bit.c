
#include <avr/interrupt.h> // AVR 인터럽트에 대한 헤더파일
#include <avr/io.h>        // AVR 입출력에 대한 헤더 파일

volatile unsigned char LED_Data = 0x00;
unsigned char timer1Cnt = 0;

int main(void)
{
    DDRC = 0x0F; // 포트C 를 출력포트로 설정한다.

    TCCR1A = 0x00;
    TCCR1B = 0x01; // 프리스케일러 1;

    TCNT1 = 0; // 0 -> 1/225초 마다 한번씩 인터럽트 발생
    TIMSK = 0x04;
    TIFR |= 1 << TOV1;

    sei();

    while (1)
    {
        PORTC = LED_Data; // 포트C로 변수 LED_Data에 있는 데이타를 출력한다.
    }
}

SIGNAL(TIMER1_OVF_vect)
{
    cli();

    timer1Cnt++; // timer0Cnt 변수를 1 증가 시킨다.

    if (timer1Cnt == 225)
    {               // 1/255s * 255 = 1s  //1초를 얻기 위한 카운트 횟수
        LED_Data++; // LED_Data 변수를 1 증가 시킨다.
        if (LED_Data > 0x0F)
            LED_Data = 0;
        timer1Cnt = 0;
    }
    sei();
}
