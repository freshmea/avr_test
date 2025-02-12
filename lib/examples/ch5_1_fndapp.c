
#include <avr/interrupt.h> // AVR 인터럽트에 대한 헤더파일
#include <avr/io.h>        // AVR 입출력에 대한 헤더 파일
#include <util/delay.h>    // delay 함수사용을 위한 헤더파일

volatile unsigned char Time_STOP = 0;
volatile unsigned char cnt = 0; // FND Table 카운터 변수

int main(void)
{
    // 7-Segment에 표시할 글자의 입력 데이터를 저장
    unsigned char FND_DATA_TBL[] = {0x3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7C, 0X07, 0X7F, 0X67, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71, 0X08, 0X80};

    DDRA = 0xFF; // 포트A 를 출력포트로 설정한다.
    DDRE = 0x00; // 포트E 를 입력포트로 설정한다.

    EICRB = 0x32; // 인터럽트 4를 하강에지, 6을 상승에지에서 동작하도록 설정한다.
    EIMSK = 0x50; // 인터럽트 4, 6을 허용
    EIFR = 0x50;  // 인터럽트 4, 6 플래그를 클리어

    sei(); // 전체 인터럽트를 허용

    while (1)
    {
        PORTA = FND_DATA_TBL[cnt]; // 포트A에 FND Table 값을 출력한다.

        if (Time_STOP == 0) // Time_Stop이 0인경우에만
        {
            cnt++; // FND Table 카운터 변수를 증가한다.
            if (cnt > 17)
                cnt = 0; // 테이블 크기를 초과하는 경우 방지.
        }
        _delay_ms(200);
    }
}

SIGNAL(INT4_vect) // 인터럽트 서비스 루틴
{
    cli(); // 전체 인터럽트를 금지

    Time_STOP = 1; // Time_Stop에 1을 입력한다.
    cnt = 0;       // FND Table 카운터 변수 값을 초기화한다.

    sei(); // 전체 인터럽트를 허용
}

SIGNAL(INT6_vect) // 인터럽트 서비스 루틴
{
    cli(); // 전체 인터럽트를 금지

    Time_STOP = 0; // Time_Stop에 0을 입력한다.

    sei(); // 전체 인터럽트를 허용
}
