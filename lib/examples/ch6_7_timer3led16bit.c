
#include <avr/interrupt.h> // AVR 인터럽트에 대한 헤더파일
#include <avr/io.h>        // AVR 입출력에 대한 헤더 파일

volatile unsigned char LED_Data = 0x00;

int main(void)
{
    DDRC = 0x0F; // 포트C 를 출력포트로 설정한다.

    TCCR3A = 0x00;
    TCCR3B = 0x05; // 프리스케일러 1024;

    TCNT3 = 51136; // 65536 - 14400 = 51136-> 1초 마다 한번씩 인터럽트 발생
    ETIMSK = 0x04;
    ETIFR |= 1 << TOV3;

    sei();

    while (1)
    {
        PORTC = LED_Data; // 포트C로 변수 LED_Data에 있는 데이타를 출력한다.
    }
}

SIGNAL(TIMER3_OVF_vect)
{
    cli();

    TCNT3 = 51136; // 65536 - 14400 = 51136-> 1초 마다 한번씩 인터럽트 발생

    LED_Data++; // LED_Data 변수를 1 증가 시킨다.
    if (LED_Data > 0x0F)
        LED_Data = 0;

    sei();
}
