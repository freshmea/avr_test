
#include "lcd.h"    //Text LCD를 사용하기 위한 헤더 파일
#include <avr/io.h> //AVR 입출력에 대한 헤더 파일

unsigned char cursor = 0;

void putch(unsigned char data)
{
    while ((UCSR0A & 0x20) == 0)
        ;        // 전송준비가 될때까지 대기
    UDR0 = data; // 데이터를 UDR0에 쓰면 전송된다
    UCSR0A |= 0x20;
}

unsigned char getch(void)
{
    unsigned char data;
    while ((UCSR0A & 0x80) == 0)
        ;        // 데이타를 받을때까지 대기
    data = UDR0; // 수신된 데이터는 UDR0에 저장되어 있다.
    UCSR0A |= 0x80;
    return data; // 읽어온 문자를 반환한다.
}

int main(void)
{
    unsigned char RX_data = 0;
    DDRE = 0x0e; // Rx(입력 0), Tx(출력, 1), SW0 ~ 3 입력

    UCSR0A = 0x00;
    UCSR0B = 0x18; // Rx, Tx enable
    UCSR0C = 0x06; // 비동기 방식, No Parity bit, 1 Stop bit

    UBRR0H = 0x00;
    UBRR0L = 0x07; // 115200 bps

    lcdInit(); // Text LCD를 초기화

    while (1)
    {
        RX_data = getch(); // PC로 입력받은 데이타를 변수 RX_data로 저장
        if (cursor == 0)
        {
            lcdClear(); // LCD에 출력된 문자를 클리어한다.
        }
        putch(RX_data);        // 변수 RX_data로 저장된 데이타를 PC로 출력
        lcdDataWrite(RX_data); // 변수 RX_data로 저장된 데이타를 LCD로 출력
        cursor++;              // LCD로 출력한 데이타를 카운트한다.
        if (cursor == 16)      // LCD로 카운트한값이 16이면
            lcdGotoXY(0, 1);   // 커서를 2행을 이동한다.
        else if (cursor == 32) // LCD로 카운트한값이 32이면
            cursor = 0;        // 변수 cursor 값을 으로 초기화 한다.
    }
}
