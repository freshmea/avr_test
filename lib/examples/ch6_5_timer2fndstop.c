
#include <avr/interrupt.h> // AVR 인터럽트에 대한 헤더파일
#include <avr/io.h>        // AVR 입출력에 대한 헤더 파일

// 7-Segment에 표시할 글자의 입력 데이터를 저장
unsigned char FND_DATA_TBL[] = {0x3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7C, 0X07, 0X7F, 0X67, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71, 0X08, 0X80};
volatile unsigned char time_s = 0;    // 0.5초를 세는 변수
volatile unsigned char Time_STOP = 0; // 숫자 증가 동작 및 정지시키는 변수
unsigned char timer0Cnt = 0;

int main(void)
{
    DDRA = 0xFF; // 포트A 를 출력포트로 설정한다.
    DDRE = 0x00; // 포트E 를 입력포트로 설정한다.

    TCCR2 = 0x05; // 프리스케일러 1024
    OCR2 = 144;   // 0.01초 마다 한번씩 인터럽트 발생
    TIMSK = 0x80;
    TIFR |= 1 << OCF2;

    EICRB = 0x03; // 인터럽트 4를 상승엣지에서 동작하도록 설정한다.
    EIMSK = 0x10; // 인터럽트 4를 허용
    EIFR = 0x10;  // 인터럽트 4 플래그를 클리어

    sei();

    while (1)
    {
        PORTA = FND_DATA_TBL[time_s]; // 포트A에 FND Table 값을 출력한다.
    }
}

SIGNAL(TIMER2_COMP_vect)
{
    cli(); // 전체 인터럽트를 금지

    OCR2 += 144; // 0.01초 후에 인터럽트 발생
    timer0Cnt++; // timer0Cnt 변수를 1 증가 시킨다.

    if (timer0Cnt == 50)
    {                       // 0.01s * 50 = 0.5s  //0.5초를 얻기 위한 카운트 횟수
        if (Time_STOP == 0) // Time_Stop이 0인경우에만
        {
            if (time_s >= 16)
                time_s = 0; // time_s 변수는 16까지만 증가
            else
                time_s++; // 16되면 0으로 초기화 한다.
        }
        timer0Cnt = 0;
    }
    sei(); // 전체 인터럽트를 허용
}

SIGNAL(INT4_vect) // 인터럽트 서비스 루틴
{
    cli(); // 전체 인터럽트를 금지

    if (Time_STOP == 0) // Time_Stop이 0인경우에만
    {
        Time_STOP = 1; // Time_Stop에 1을 입력한다.
    }
    else               // Time_Stop이 1인경우에만
        Time_STOP = 0; // Time_Stop에 0을 입력한다.

    sei(); // 전체 인터럽트를 허용
}