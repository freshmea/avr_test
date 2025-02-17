#include <avr/interrupt.h> // 인터럽트 사용을 위한 헤더파일
#include <avr/io.h>        // AVR 입출력에 대한 헤더 파일
#include <stdio.h>         // sprintf 함수 사용을 위한 헤더
#include <util/delay.h>    // delay 함수사용을 위한 헤더파일

// 시스템 밀리세컨드 타이머
volatile uint32_t g_millis = 0;

// Timer1 Compare Match 인터럽트: 1ms마다 g_millis 증가
ISR(TIMER0_OVF_vect)
{
  g_millis++;
}

uint32_t millis(void)
{
  uint32_t ms;
  cli();
  ms = g_millis;
  sei();
  return ms;
}

void timer0_init(void)
{
  TCCR0 = (1 << CS02);
  TIMSK |= (1 << TOIE0);
}

// 비차단 방식으로 스위치 디바운싱 (active low)
// 매개변수 pin: 포트E의 핀 번호 (0~3)
// 내부에 static 배열을 사용해 각 핀의 상태를 독립적으로 관리.
uint8_t readSwitchNonBlocking(uint8_t pin)
{
  // static 변수: 각 핀 별 마지막 안정 상태, 마지막 읽은 상태, 마지막 상태 변화 시간
  static uint8_t lastStable[8] = {0};
  static uint8_t lastReading[8] = {0};
  static uint32_t lastDebounce[8] = {0};

  // 현재 읽은 원시값 (active low: 눌리면 0)
  uint8_t raw = (PINE & (1 << pin)) ? 1 : 0;
  // pressed: 1이면 버튼이 눌림 (raw==0)
  uint8_t pressed = (raw == 0) ? 1 : 0;

  // 상태 변화 감지
  if (pressed != lastReading[pin])
  {
    lastDebounce[pin] = millis(); // 변화 최초 시각 기록
  }
  lastReading[pin] = pressed;

  // 20ms 이상 상태가 안정되었다면
  if ((millis() - lastDebounce[pin]) > 20)
  {
    if (pressed != lastStable[pin])
    {
      lastStable[pin] = pressed;
      // 여기서만 눌림 이벤트를 반환 (버튼 눌림 시 한 번만 1 반환)
      if (pressed == 1)
        return 1;
    }
  }
  return 0;
}

int main(void)
{
  // PORTC 하위 4비트(LED 0~3)를 출력으로 설정
  DDRC = 0x0F;
  // PORTE 모든 핀에 내부 풀업 활성화
  PORTE = 0xFF;

  timer0_init(); // 타이머 초기화
  sei();         // 전역 인터럽트 활성화

  while (1)
  {
    // E포트의 0~3 스위치 상태를 비차단 방식으로 체크하여 PORTC LED 토글
    if (readSwitchNonBlocking(0))
      PORTC ^= _BV(PC0);
    if (readSwitchNonBlocking(1))
      PORTC ^= _BV(PC1);
    if (readSwitchNonBlocking(2))
      PORTC ^= _BV(PC2);
    if (readSwitchNonBlocking(3))
      PORTC ^= _BV(PC3);
  }
}
