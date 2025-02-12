
#include <avr/interrupt.h> // AVR 인터럽트에 대한 헤더파일
#include <avr/io.h>        // AVR 입출력에 대한 헤더 파일

// 7-Segment에 표시할 글자의 입력 데이터를 저장
unsigned char FND_DATA_TBL[] = {0x3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7C, 0X07, 0X7F, 0X67, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71, 0X08, 0X80};
volatile unsigned char time_s = 0; // 초를 세는 변수
volatile unsigned char FND_flag = 0, edge_flag = 0;

int main(void)
{
    DDRA = 0xFF; // 포트A 를 출력포트로 설정한다.
    DDRE = 0x00; // 포트E 를 입력포트로 설정한다.

    TCCR3A = 0x00;
    TCCR3B = 0x45; // 프리스케일러 1024, 상승(양)에지 캡처 트리거 설정

    ETIMSK = 0x20;
    ETIFR |= 1 << ICF3;

    sei();

    PORTA = FND_DATA_TBL[0]; // 포트A에 FND Table의 값을 출력한다.

    while (1)
    {
        if (FND_flag)
        {
            if (time_s > 15) // 최대 1.5초까지 표시(F)
                time_s = 15;
            PORTA = FND_DATA_TBL[time_s]; // 포트A에 FND Table 값을 출력한다.
            FND_flag = 0;
        }
    }
}

SIGNAL(TIMER3_CAPT_vect)
{
    cli();

    unsigned int count_check;

    // 스위치가 눌릴 시간 측정을 위해 상승에지에서 하강에지까지의 시간을 계산
    if (edge_flag == 0) // 상승 에지(스위치를 누르면)
    {
        TCCR3B = 0x05; // 프리스케일러 1024, 하강(음)에지 캡처 트리거 설정
        TCNT3 = 0;     // TCNT3 레지스터를 0으로 초기화
        ICR3 = 0;      // ICR3 레지스터를 0으로 초기화
        edge_flag = 1;
    }
    else // 하강 에지(스위치를 떼면)
    {
        TCCR3B = 0x45; // 프리스케일러 1024, 상승(양)에지 캡처 트리거 설정
        count_check = ICR3;
        time_s = count_check / 1440; // 누를 시간을 0.1초 단위로 변경
        FND_flag = 1;                // 측정 시간 FND로 출력 0 ~ 1.5초 까지 측정 가능
        edge_flag = 0;
    }

    sei();
}
