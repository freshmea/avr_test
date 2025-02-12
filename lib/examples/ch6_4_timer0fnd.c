// Timer0 코드에서는 OCR0 값(144)을 이용해 0.01초마다 인터럽트를 발생시키고
// timer0Cnt 변수를 이용하여 100회 인터럽트(즉 1초)가 경과될 때마다 time_s 값을 증가시킵니다.

#include <avr/interrupt.h> // AVR 인터럽트에 대한 헤더파일
#include <avr/io.h>        // AVR 입출력에 대한 헤더 파일

// 7-Segment에 표시할 글자의 입력 데이터를 저장
unsigned char FND_DATA_TBL[] = {0x3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7C, 0X07, 0X7F, 0X67, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71, 0X08, 0X80};
volatile unsigned char time_s = 0; // 초를 세는 변수
unsigned char timer0Cnt = 0;

int main(void)
{
    DDRA = 0xFF; // 포트A 를 출력포트로 설정한다.

    TCCR0 = 0x07; // 프리스케일러 1024
    OCR0 = 144;   // 0.01초 마다 한번씩 인터럽트 발생
    TIMSK = 0x02; // 출력비교 인터럽트 활성화
    TIFR |= 1 << OCF0;

    sei();

    while (1)
    {
        PORTA = FND_DATA_TBL[time_s]; // 포트A에 FND Table 값을 출력한다.
    }
}

SIGNAL(TIMER0_COMP_vect)
{
    cli();
    OCR0 += 144; // 0.01초 후에 인터럽트 발생
    timer0Cnt++; // timer0Cnt 변수를 1 증가 시킨다.

    if (timer0Cnt == 100)
    { // 0.01s * 100 = 1s  //1초를 얻기 위한 카운트 횟수
        if (time_s >= 16)
            time_s = 0; // time_s 변수는 16까지만 증가
        else
            time_s++; // 16되면 0으로 초기화 한다.
        timer0Cnt = 0;
    }
    sei();
}
