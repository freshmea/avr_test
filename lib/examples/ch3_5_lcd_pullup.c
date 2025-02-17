#include "lcd.h"        // 추가: LCD 함수 사용을 위한 헤더
#include <avr/io.h>     //AVR 입출력에 대한 헤더 파일
#include <stdio.h>      // sprintf 함수를 위한 헤더
#include <util/delay.h> //delay 함수사용을 위한 헤더파일

int main(void)
{
    unsigned char switch_flag = 0;
    char lcdBuffer[17]; // 추가: LCD 출력용 버퍼

    DDRC = 0x0F;  // 포트C 를 출력포트로 설정한다.
    PORTE = 0x0F; // 포트E 스위치를 풀업으로 설정한다.

    lcdInit();  // 추가: LCD 초기화
    lcdClear(); // 추가: LCD 화면 클리어

    while (1)
    {

        if (PINE) // 포트 E로 입력받은 값이 있을때
        {
            switch_flag = PINE; // 포트 E로 입력받은 값을 변수 Switch_flag에 저장한다.
        }

        PORTC ^= 0x0F; // PORTC(LED가 연결된 포트)의 하위 4비트를 반전시키다.

        // 첫 번째 줄에 switch_flag 값을 표시
        lcdGotoXY(0, 0);
        snprintf(lcdBuffer, sizeof(lcdBuffer), "SW: 0x%02X", switch_flag);
        lcdPrint(lcdBuffer);

        // 두 번째 줄에 MCUCR의 PUD 비트 값을 표시 (PUD: 풀업 비활성화 비트)
        lcdGotoXY(0, 1);
        snprintf(lcdBuffer, sizeof(lcdBuffer), "PUD: 0x%02X", PINE);
        lcdPrint(lcdBuffer);

        if (switch_flag & _BV(PC0))
            _delay_ms(125); // SW0을 눌렀을때
        if (switch_flag & _BV(PC1))
            _delay_ms(250); // SW1을 눌렀을때
        if (switch_flag & _BV(PC2))
            _delay_ms(375); // SW2을 눌렀을때
        if (switch_flag & _BV(PC3))
            _delay_ms(500); // SW3을 눌렀을때
                            // 두개 이상의 스위치도 동시에 눌릴 수 있음
    }
}
