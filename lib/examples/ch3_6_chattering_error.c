#include <avr/io.h> //AVR 입출력에 대한 헤더 파일
#include <stdio.h>  // sprintf 함수를 위한 헤더

int main(void)
{
    unsigned char switch_flag = 0;

    DDRC = 0x01;  // 포트C 를 출력포트로 설정한다.
    PORTE = 0x01; // 포트E 스위치를 풀업으로 설정한다.

    while (1)
    {

        if (PINE) // 포트 E로 입력받은 값이 있을때
        {
            switch_flag = PINE; // 포트 E로 입력받은 값을 변수 Switch_flag에 저장한다.
        }

        if (!(switch_flag & _BV(PC0)))
            PORTC ^= 0x01; // 반전
    }
}
