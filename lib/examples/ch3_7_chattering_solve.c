#include <avr/io.h>     // AVR 입출력에 대한 헤더 파일
#include <util/delay.h> // delay 함수사용을 위한 헤더파일

int main(void)
{
    DDRC = 0x01;  // PORTC의 비트0을 출력으로 설정 (LED 연결)
    PORTE = 0xFF; // PORTE의 모든 핀에 내부 풀업 활성화

    while (1)
    {
        // 버튼(PINE의 PC0에 연결, active low) 입력 읽기 및 디바운스 처리
        if (!(PINE & _BV(PC0))) // 버튼 눌림 판정
        {
            _delay_ms(20);          // 디바운스 대기
            if (!(PINE & _BV(PC0))) // 다시 확인
            {
                PORTC ^= _BV(PC0); // LED 토글

                // 버튼이 놓일 때까지 대기 (추가 채터링 방지)
                while (!(PINE & _BV(PC0)))
                {
                    _delay_ms(10);
                }
            }
        }
    }
}
