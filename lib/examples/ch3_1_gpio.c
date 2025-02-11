
#include <avr/io.h>     // AVR 입출력에 대한 헤더 파일
#include <util/delay.h> // delay 함수사용을 위한 헤더파일

int main(void)
{
    while (1)
    {
        DDRC = 0x03;    // LED0,1 포트 출력, LED2,3 입력으로 설정
        PORTC = 0x0F;   // 모두 1을 출력
                        // 입력일 경우에는 해당 포트를 풀업으로 설정된다.
        _delay_ms(500); // 500ms 시간 지연
        PORTC = 0x00;   // 모두 0을 출력
                        // 입력일 경우에는 해당 포트를 풀다운으로 설정된다.
        _delay_ms(500); // 500ms 시간 지연
        DDRC = 0x0C;    // LED0,1 포트 입력, LED2,3 출력으로 설정
        PORTC = 0x0F;   // 모두 1을 출력
                        // 입력일 경우에는 해당 포트를 풀업으로 설정된다.
        _delay_ms(500); // 500ms 시간 지연
        PORTC = 0x00;   // 모두 0을 출력
                        // 입력일 경우에는 해당 포트를 풀 다운으로 설정된다.
        _delay_ms(500); // 500ms 시간 지연
    }
}
