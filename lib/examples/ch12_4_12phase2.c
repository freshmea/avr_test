
#include <avr/interrupt.h> // AVR 인터럽트에 대한 헤더파일
#include <avr/io.h>        //AVR 입출력에 대한 헤더 파일

#define DIR_L 0
#define DIR_R 1

unsigned char timer0Cnt = 0, mot_cnt = 0;
volatile unsigned char dir = DIR_R; // 처음방향은 우측부터
// 1-2 상 여자 값을 사용
unsigned char Step[] = {0x90, 0x80, 0xC0, 0x40, 0x60, 0x20, 0x30, 0x10};

SIGNAL(TIMER0_OVF_vect); // Timer0 Overflow0 ISP
SIGNAL(TIMER1_OVF_vect); // Timer1 Overflow1 ISP

int main(void)
{
    DDRB = 0x20; // MOTOR1_EN 를 출력 포트로 설정 한다.
    DDRD = 0xF0; // STEP0, STEP1, STEP2, STEP3을 출력 포트로 설정 한다.

    PORTB &= ~0x20; // M1 Disable, DC 모터 정지

    TCCR0 = 0x07;
    TCNT0 = 112; // 256-144=112 -> 0.01초 마다 한번씩 인터럽트 발생

    TCCR1A = 0;
    TCCR1B = 0x04; //(1 / (14.7456 Mhz  / 256 prescaler )) * 1267 = 21.99ms => 22ms -> 45khz
    TCNT1H = 0xFB; // 65536-1267 = 64269
    TCNT1L = 0x0D; // 64269 = 0xFB0D

    TIMSK = 0x05; // TOIE1, TOIE0 모두 '1'
    TIFR = 0x05;  // TOV1, TOV0 모두 '1'
    sei();

    while (1)
    {
    }
}

SIGNAL(TIMER0_OVF_vect)
{
    cli();

    TCNT0 = 112; // 256-144=122 -> 0.01초 마다 한번씩 인터럽트 발생
    timer0Cnt++; // timer0Cnt 변수를 1 증가 시킨다.

    if (timer0Cnt == 200) // 0.01s * 200 = 2s  //1초를 얻기 위한 카운트 횟수
    {
        dir ^= 1;      // 방향 전환
        timer0Cnt = 0; // timer0Cnt 카운터 초기화
    }
    sei();
}

SIGNAL(TIMER1_OVF_vect)
{
    cli();

    TCNT1H = 0xFB; // 22ms
    TCNT1L = 0x0D;
    PORTD = Step[mot_cnt]; // 1-2상 여자 방식 한 스텝 진행

    if (dir == DIR_R) // 회전 방향이 우측 방향이면
    {
        if (mot_cnt++ == 7)
            mot_cnt = 0; // 스텝 카운터 증가
    }
    else if (mot_cnt-- == 0)
        mot_cnt = 7; // 스텝 카운터 감소

    sei();
}
