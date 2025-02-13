
#include "SHT2x.h"      //SHT2x 라이브러리 사용을 위한 헤더 파일
#include "TWI_driver.h" //TWI 사용을 위한 헤더 파일
#include "lcd.h"        //Text LCD를 사용하기 위한 헤더 파일
#include <avr/io.h>     //AVR 입출력에 대한 헤더 파일
#include <util/delay.h> //delay 함수사용을 위한 헤더파일

void printf_2dot1(uint8_t sense, uint16_t sense_temp); // 온도 및 습도를 LCD에 출력하는 함수
uint16_t temperatureC, humidityRH;                     // 온도, 습도 값 측정에 사용되는 변수

int main(void)
{
    uint8_t error = 0; // 에러를 저장하는 변수

    nt16 sRH; // 습도의 raw 데이터를 저장하는 변수
    nt16 sT;  // 온도의 raw 데이터를 저장하는 변수

    Init_TWI();   // TWI를 초기화 한다
    lcdInit();    // Text LCD를 초기화
    SHT2x_Init(); // SHT 센서를 초기화 한다

    while (1)
    {
        error |= SHT2x_MeasureHM(HUMIDITY, &sRH); // 습도를 측정한다.
        error |= SHT2x_MeasureHM(TEMP, &sT);      // 온도를 측정한다.
        // 온도 습도를 계산, 소숫점 첫째자리까지 출력하기 위해 10을 곱한다.
        temperatureC = SHT2x_CalcTemperatureC(sT.u16) * 10; // 온도를 계산한다.
        humidityRH = SHT2x_CalcRH(sRH.u16) * 10;            // 습도를 계산한다.

        if (error == SUCCESS) // 에러없이 정상 측정 되었으면
        {
            lcdGotoXY(0, 0);                    // 현재 커서위치를 첫번째줄 첫번째칸으로 이동한다.
            printf_2dot1(TEMP, temperatureC);   // 온도를 출력한다.
            lcdGotoXY(0, 1);                    // 현재 커서위치를 두번째줄 첫번째칸으로 이동한다.
            printf_2dot1(HUMIDITY, humidityRH); // 습도를 출력한다.
        }
        else // 에러가 있을 경우
        {
            lcdGotoXY(0, 0);                  // 현재 커서위치를 첫번째줄 첫번째칸으로 이동한다.
            lcdPrintData(" Temp: --.-C", 12); // 온도를 --.-로 출력한다.
            lcdGotoXY(0, 1);                  // 현재 커서위치를 두번째줄 첫번째칸으로 이동한다.
            lcdPrintData(" Humi: --.-%", 12); // 습도를 --.-로 출력한다.
        }
        _delay_ms(300); // 다음 측정을 위한 시간 지연(300ms)
    }
}

void printf_2dot1(uint8_t sense, uint16_t sense_temp)
{
    uint8_t s100, s10;

    if (sense == TEMP)
        lcdPrintData(" Temp: ", 7); // 온도 출력시 " Temp: " 출력
    else if (sense == HUMIDITY)
        lcdPrintData(" Humi: ", 7); // 습도 출력시 " Humi: " 출력

    s100 = sense_temp / 100; // 100의 자리 추출

    if (s100 > 0)
        lcdDataWrite(s100 + '0'); // 100의 자리 값이 있으면 100의 자리 출력
    else
        lcdPrintData(" ", 1); // 100의 자리 값이 없으면 빈칸 출력

    s10 = sense_temp % 100;         // 100의 자리를 제외한 나머지 추출
    lcdDataWrite((s10 / 10) + '0'); // 10의 자리 추출하여 출력
    lcdPrintData(".", 1);           // 소숫점 출력
    lcdDataWrite((s10 % 10) + '0'); // 1의 자리 추출하여 출력

    if (sense == TEMP)
        lcdDataWrite('C'); // 온도 단위 출력
    else if (sense == HUMIDITY)
        lcdDataWrite('%'); // 습도 단위 출력
}
