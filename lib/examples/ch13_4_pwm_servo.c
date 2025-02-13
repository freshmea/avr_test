
#include <avr/interrupt.h> // AVR 인터럽트에 대한 헤더파일
#include <avr/io.h>        //AVR 입출력에 대한 헤더 파일
#include <util/delay.h>    //delay 함수사용을 위한 헤더파일

int timer0Cnt = 0;
int AngleCount = 0;

void Servo(int);
SIGNAL(TIMER0_OVF_vect); // Timer0 Overflow0 ISP

int main(void)
{
    DDRE = 0x08;     // SERVO 핀을 출력 포트로 설정 한다.
    AngleCount = 23; // 초기 서보 위치를 +90도로 설정한다.

    TCCR0 = 0x02; // 8분주
    TCNT0 = 0x48; // 256(0x100)-72(0x48)=184(0xB8) -> 0.1ms 마다 한번씩 인터럽트 발생

    TIMSK = 0x01; // TOIE0 '1'
    TIFR = 0x01;  // TOV0 '1'
    sei();

    while (1)
    {
        // delay 1초를 주는 이유는 서보가 움직일 충분한 시간을 주기 위함
        Servo(90);
        _delay_ms(1000);
        Servo(0);
        _delay_ms(1000);
        Servo(-90);
        _delay_ms(1000);
    }
}

void Servo(int Angle)
{
    if (Angle == -90)
        AngleCount = 24; // PWM ON Time 2.4[ms]
    else if (Angle == 0)
        AngleCount = 15; // PWM ON Time 1.5[ms]
    else if (Angle == 90)
        AngleCount = 6; // PWM ON Time 0.6[ms]
}

SIGNAL(TIMER0_OVF_vect)
{
    cli();

    TCNT0 = 0x48; // 256(0x100)-72(0x48)=184(0xB8) -> 0.1ms 마다 한번씩 인터럽트 발생
    timer0Cnt++;  // timer0Cnt 변수를 1 증가 시킨다.

    if (timer0Cnt <= AngleCount)
    {
        PORTE |= 0x08; // PWM ON Time
    }
    else
        PORTE &= ~0x08; // PWM OFF Time

    if (timer0Cnt >= 200)
        timer0Cnt = 0; // timer0Cnt 카운터 초기화

    sei();
}
