#include "lcd.h" // 변경: lcd.h 헤더 사용
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

int main(void)
{
  DDRE = 0x08; // SERVO 핀(PE3)을 출력으로 설정

  // Timer3 Fast PWM mode14, non-inverting, 분주비 8 설정
  TCCR3A = (1 << COM3A1) | (1 << WGM31);
  TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS31);
  ICR3 = 40000; // 20ms 주기 (20ms / 0.5µs)
  OCR3A = 3000; // 1.5ms 펄스폭 (1.5ms / 0.5µs)

  lcdInit();  // 변경: LCD 초기화 함수 사용
  lcdClear(); // 변경: LCD 클리어 함수 사용

  while (1)
  {
    // 펄스폭을 0.6ms (1200 카운트)에서 2.4ms (4000 카운트)까지 증가
    for (uint16_t pulse = 1000; pulse <= 4500; pulse += 40)
    {
      OCR3A = pulse;
      { // 추가: LCD에 펄스폭 값 업데이트 (숫자만 갱신)
        char buffer[16];
        itoa(pulse, buffer, 10);
        lcdGotoXY(0, 7); // Pulse: 문자열 뒤에 출력 (행, 열)
        lcdPrint(buffer);
        lcdPrint("     "); // 남은 자리 지우기
      }
      _delay_ms(20);
    }
    // 펄스폭을 2.4ms (4000 카운트)에서 0.6ms (1200 카운트)로 감소
    for (uint16_t pulse = 4500; pulse >= 1000; pulse -= 40)
    {
      OCR3A = pulse;
      { // 추가: LCD에 펄스폭 값 업데이트
        char buffer[16];
        itoa(pulse, buffer, 10);
        lcdGotoXY(0, 7);
        lcdPrint(buffer);
        lcdPrint("     ");
      }
      _delay_ms(20);
    }
  }
}
