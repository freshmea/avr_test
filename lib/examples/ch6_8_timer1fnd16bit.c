// Timer1 코드에서는 OCR1A 값을 14400으로 설정하여 인터럽트가 1초마다 직접 발생하므로
// 별도의 카운트 변수 없이 time_s 값을 바로 증가시킵니다.

#include <avr/interrupt.h> // AVR 인터럽트에 대한 헤더파일
#include <avr/io.h>        // AVR 입출력에 대한 헤더 파일

// 7-Segment에 표시할 글자의 입력 데이터를 저장
unsigned char FND_DATA_TBL[] = {0x3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7C, 0X07, 0X7F, 0X67, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71, 0X08, 0X80};
volatile unsigned char time_s = 0; // 초를 세는 변수
unsigned char timer0Cnt = 0;

int main(void)
{
    DDRA = 0xFF; // 포트A 를 출력포트로 설정한다.

    TCCR1A = 0x00;
    TCCR1B = 0x05; // 프리스케일러 1024;

    OCR1A = 14400; // 1초 마다 한번씩 인터럽트 발생
    TIMSK = 0x10;
    TIFR |= 1 << OCF1A;

    sei();

    while (1)
    {
        PORTA = FND_DATA_TBL[time_s]; // 포트A에 FND Table 값을 출력한다.
    }
}

SIGNAL(TIMER1_COMPA_vect)
{
    cli();

    OCR1A += 14400; // 1초 후에 인터럽트 발생

    if (time_s >= 17)
        time_s = 0; // time_s 변수는 17까지만 증가
    else
        time_s++; // 17되면 0으로 초기화 한다.

    sei();
}
