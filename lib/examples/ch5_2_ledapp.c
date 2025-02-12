
#include <avr/interrupt.h> // AVR 인터럽트에 대한 헤더파일
#include <avr/io.h>        // AVR 입출력에 대한 헤더 파일
#include <util/delay.h>    // delay 함수사용을 위한 헤더파일

volatile unsigned char Shift_flag = 1;

int main(void)
{
    unsigned char LED_Data = 0x01;

    DDRC = 0x0F; // 포트C 를 출력포트로 설정한다.
    DDRE = 0x00; // 포트E 를 입력포트로 설정한다.

    EICRB = 0xC8; // 인터럽트 5를 하강에지, 7을 상승에지에서 동작하도록 설정한다.
    EIMSK = 0xA0; // 인터럽트 5, 7을 허용
    EIFR = 0xA0;  // 인터럽트 5, 7 플래그를 클리어

    sei(); // 전체 인터럽트를 허용

    while (1)
    {
        PORTC = LED_Data;    // 포트C로 변수 LED_Data에 있는 데이타를 출력한다.
        if (Shift_flag == 1) // LED0 ~ LED3으로 이동
        {
            if (LED_Data == 0x08)
                LED_Data = 0x01; // LED_Data 값이 0x08 이면 LED_Data 값을 1로 한다.
            else
                LED_Data <<= 1; // LED_Data 값을 왼쪽으로 쉬프트
        }
        else if (Shift_flag == 2) // LED3 ~ LED0으로 이동
        {
            if (LED_Data == 0x01)
                LED_Data = 0x08; // LED_Data 값이 0x01 이면 LED_Data 값을 0x08로 한다.
            else
                LED_Data >>= 1; // LED_Data 값을 오른쪽으로 쉬프트
        }
        _delay_ms(100);
    }
}

SIGNAL(INT5_vect) // 인터럽트 서비스 루틴
{
    cli(); // 전체 인터럽트를 금지

    Shift_flag = 1; // Shift_flag에 1을 입력한다.

    sei(); // 전체 인터럽트를 허용
}

SIGNAL(INT7_vect) // 인터럽트 서비스 루틴
{
    cli(); // 전체 인터럽트를 금지

    Shift_flag = 2; // Shift_flag에 2를 입력한다.

    sei(); // 전체 인터럽트를 허용
}
