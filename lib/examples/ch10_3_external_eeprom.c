
#include "at25160.h"    //AT25160B 라이브러리 사용을 위한 헤더 파일
#include "lcd.h"        //Text LCD를 사용하기 위한 헤더 파일
#include <avr/io.h>     //AVR 입출력에 대한 헤더 파일
#include <util/delay.h> //delay 함수사용을 위한 헤더파일

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

unsigned char msg1[] = "Welcome!!";
unsigned char msg2[] = "edgeiLAB-World!!";
unsigned char msg3[] = "SPI-Flash Exam";

int main(void)
{
    unsigned char i = 0;
    unsigned char buf1[20] = {0};
    unsigned char buf2[20] = {0};
    unsigned char buf3[20] = {0};

    SPI_Init();
    lcdInit();

    at25160_Write_Arry(0x0100, msg1, ARRAY_SIZE(msg1)); //"Welcome!!" 저장
    at25160_Write_Arry(0x0200, msg2, ARRAY_SIZE(msg2)); //"edgeiLAB-World!!" 저장
    at25160_Write_Arry(0x0300, msg3, ARRAY_SIZE(msg3)); //"SPI-Flash Exam" 저장

    at25160_Read_Arry(0x0100, buf1, ARRAY_SIZE(msg1)); // 각 배열에다가 플래쉬에 저장한 문자열을 읽어온다.
    at25160_Read_Arry(0x0200, buf2, ARRAY_SIZE(msg2));
    at25160_Read_Arry(0x0300, buf3, ARRAY_SIZE(msg3));

    for (lcdGotoXY(0, 0); i < ARRAY_SIZE(msg1) - 1; i++)
    { // 읽어들인 문자열 출력
        lcdDataWrite(buf1[i]);
        _delay_ms(100);
    }

    for (i = 0, lcdGotoXY(0, 1); i < ARRAY_SIZE(msg2) - 1; i++)
    {
        lcdDataWrite(buf2[i]);
        _delay_ms(100);
    }

    for (i = 0; i < ARRAY_SIZE(msg3) - 1; i++)
    {
        lcdDataWrite(buf3[i]);
        _delay_ms(100);
    }

    for (i = 0; i < 16; i++)
    { // LCD 화면을 왼쪽으로 시프트
        lcdControlWrite(1 << LCD_MOVE | 1 << LCD_MOVE_DISP);
        _delay_ms(100);
    }

    while (1)
    {
    }
}
