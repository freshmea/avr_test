
#include <avr/interrupt.h> // AVR 인터럽트에 대한 헤더파일
#include <avr/io.h>        // AVR 입출력에 대한 헤더 파일

volatile int Light = 0;
volatile unsigned char Light_flag = 1;

int main(void)
{
    DDRB = 0x80; // 포트B 를 출력포트로 설정한다.
                 // LED0을 PB7과 케이블로 연결
    DDRE = 0x00; // 포트E 를 입력포트로 설정한다.

    TCCR2 = 0x62; // PC PWM 모드, 8 분주
                  // PWM 주기 : F_CPU/256/2/8 = 3.6 KHz
                  // 업카운트시 Clear, 다운카운트시 Set 으로 설정
    TCNT2 = 0x00; // 타이머2 카운터를 초기화 한다.

    EICRB = 0xFF; // 인터럽트 4, 5, 6, 7을 상승엣지에서 동작하도록 설정한다.
    EIMSK = 0xF0; // 인터럽트 4, 5, 6, 7을 허용
    EIFR = 0xF0;  // 인터럽트 4, 5, 6, 7 플래그를 클리어

    sei(); // 전체 인터럽트를 허용

    while (1)
    {
        if (Light_flag)
        {
            OCR2 = Light; // Light 값에 따라 밝기 제어
            Light_flag = 0;
        }
    }
}

SIGNAL(INT4_vect) // 인터럽트 서비스 루틴
{
    cli(); // 전체 인터럽트를 금지

    Light = 0; // LED OFF;

    Light_flag = 1;

    sei(); // 전체 인터럽트를 허용
}

SIGNAL(INT5_vect) // 인터럽트 서비스 루틴
{
    cli(); // 전체 인터럽트를 금지

    Light -= 51; // LED 밝기 감소
    if (Light < 0)
        Light = 0;

    Light_flag = 1;

    sei(); // 전체 인터럽트를 허용
}

SIGNAL(INT6_vect) // 인터럽트 서비스 루틴
{
    cli(); // 전체 인터럽트를 금지

    Light += 51; // LED 밝기 증가;
    if (Light > 255)
        Light = 255;

    Light_flag = 1;

    sei(); // 전체 인터럽트를 허용
}

SIGNAL(INT7_vect) // 인터럽트 서비스 루틴
{
    cli(); // 전체 인터럽트를 금지

    Light = 255; // LED 밝기 100%
    Light_flag = 1;

    sei(); // 전체 인터럽트를 허용
}