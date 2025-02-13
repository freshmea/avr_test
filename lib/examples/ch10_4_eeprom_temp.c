#include "SHT2x.h"         //SHT2x 라이브러리 사용을 위한 헤더 파일
#include "TWI_driver.h"    //TWI 사용을 위한 헤더 파일
#include "at25160.h"       //AT25160B 라이브러리 사용을 위한 헤더 파일
#include <avr/interrupt.h> // AVR 인터럽트에 대한 헤더파일
#include <avr/io.h>        //AVR 입출력에 대한 헤더 파일
#include <util/delay.h>    //delay 함수사용을 위한 헤더파일

volatile unsigned char Read_flag = 1, TX_flag = 0;
unsigned int timer0Cnt = 0;
unsigned int temperatureC, humidityRH; // 온도, 습도 값 측정에 사용되는 변수

// EEPROM에 읽고 쓰기 위한 데이터
typedef union
{
    unsigned int data16;
    unsigned char data8[2];
} DataDB;
DataDB Read_Data;  // EEPROM 읽기 버퍼
DataDB Write_Data; // EEPROM 쓰기 버퍼

void putch(unsigned char data)
{
    while ((UCSR0A & 0x20) == 0)
        ;        // 전송준비가 될때까지 대기
    UDR0 = data; // 데이터를 UDR0에 쓰면 전송된다
    UCSR0A |= 0x20;
}

// 문자열을 출력하는 함수
void putch_Str(char *str)
{
    unsigned char i = 0;
    while (str[i] != '\0')
        putch(str[i++]); // 문자열을 출력
}
int main(void)
{
    unsigned char error = 0; // 에러를 저장하는 변수

    nt16 sRH; // 습도의 raw 데이터를 저장하는 변수
    nt16 sT;  // 온도의 raw 데이터를 저장하는 변수

    SPI_Init();

    Init_TWI();   // TWI를 초기화 한다
    SHT2x_Init(); // SHT 센서를 초기화 한다

    DDRE = 0x02; // Rx(입력 0), Tx(출력, 1), SW0입력

    UCSR0A = 0x00;
    UCSR0B = 0x18; // Rx, Tx enable
    UCSR0C = 0x06; // 비동기 방식, No Parity bit, 1 Stop bit
    UBRR0H = 0x00;
    UBRR0L = 0x07; // 115200 bps

    EICRB = 0x03; // 인터럽트 4을 상승에지에서 동작하도록 설정한다.
    EIMSK = 0x10; // 인터럽트 4을 허용
    EIFR = 0x10;  // 인터럽트 4 플래그를 클리어

    TCCR0 = 0x07;
    TCNT0 = 112; // 256-144=112 -> 0.01초 마다 한번씩 인터럽트 발생
    TIMSK = 0x01;
    TIFR |= 1 << TOV0;

    sei(); // 전체 인터럽트를 허용
    while (1)
    {
        if (Read_flag)
        {
            error |= SHT2x_MeasureHM(HUMIDITY, &sRH);
            // 습도를 측정한다.
            error |= SHT2x_MeasureHM(TEMP, &sT);
            // 온도를 측정한다.
            // 온도 습도를 계산, 소숫점 첫째자리까지 출력하기 위해 10을 곱한다.
            temperatureC = SHT2x_CalcTemperatureC(sT.u16) * 10;
            // 온도를 계산한다.
            humidityRH = SHT2x_CalcRH(sRH.u16) * 10; // 습도를 계산한다.
            if (error == SUCCESS)                    // 에러없이 정상 측정 되었으면
            {
                Write_Data.data16 = temperatureC;
                at25160_Write_Arry(0x0100, Write_Data.data8, 2);
                // EEPROM에 온도 저장
                Write_Data.data16 = humidityRH;
                at25160_Write_Arry(0x0200, Write_Data.data8, 2);
                // EEPROM에 습도 저장
            }
            Read_flag = 0;
        }
        if (TX_flag)
        {
            at25160_Read_Arry(0x0100, Read_Data.data8, 2);
            // EEPROM에서 온도 읽음
            temperatureC = Read_Data.data16;
            at25160_Read_Arry(0x0200, Read_Data.data8, 2);
            // EEPROM에서 습도 읽음
            humidityRH = Read_Data.data16;
            putch_Str("\n\r Temp: ");
            // 온도 출력
            // 100의 자리 값 확인
            if (temperatureC / 100) // 100의 자리가 0이 아닐 경우
            {
                // 100의 자리 출력
                putch(temperatureC / 100 + '0');
            }
            putch((temperatureC / 10) % 10 + '0');
            putch('.');
            putch(temperatureC % 10 + '0');
            putch_Str("[C], Humi : ");
            // 습도 출력
            // 100의 자리 값 확인
            if (humidityRH / 100) // 100의 자리가 0이 아닐 경우
            {
                // 100의 자리 출력
                putch(humidityRH / 100 + '0');
            }
            putch((humidityRH / 10) % 10 + '0');
            putch('.');
            putch(humidityRH % 10 + '0');
            putch_Str("[%]");
            TX_flag = 0;
        }
    }
}

// 추가: Timer0 오버플로우 인터럽트 서비스 루틴 추가
ISR(TIMER0_OVF_vect)
{
    timer0Cnt++;
    if (timer0Cnt >= 500) // 0.01초 * 500 ≒ 5초 경과
    {
        timer0Cnt = 0;
        Read_flag = 1;
    }
    TCNT0 = 112; // 다음 인터럽트를 위해 TCNT0 재설정
}

// 추가: INT4 인터럽트 서비스 루틴 구현 (예시)
ISR(INT4_vect)
{
    TX_flag = 1;
}