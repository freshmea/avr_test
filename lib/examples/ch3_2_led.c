
#include <avr/io.h>     //AVR 입출력에 대한 헤더 파일
#include <util/delay.h> //delay 함수사용을 위한 헤더파일

int main(void)
{
    unsigned char LED_Data = 0x01, i;

    DDRC = 0x0F; // 포트C 를 출력포트로 설정한다.

    while (1)
    {
        LED_Data = 0x01; // 변수 LED_Data에 초기값 0x01로 저장
        // LED0 ~ LED3으로 이동시키기
        for (i = 0; i < 4; i++)
        {
            PORTC = LED_Data; // 포트C로 변수 LED_Data에 있는 데이타를 출력한다.
            LED_Data <<= 1;   // 변수 LED_Data에 값을 좌측으로 쉬프트한다.
            _delay_ms(1000);  // ms 단위의 딜레이 함수
        }
        LED_Data = 0x08; // 변수 LED_Data에 초기값 0x08로 저장
        // LED3 ~ LED0으로 이동시키기
        for (i = 0; i < 4; i++)
        {
            PORTC = LED_Data; // 포트C로 변수 LED_Data에 있는 데이타를 출력한다.
            LED_Data >>= 1;   // 변수 LED_Data에 값을 좌측으로 쉬프트한다.
            _delay_ms(1000);  // ms 단위의 딜레이 함수
        }
    }
}
