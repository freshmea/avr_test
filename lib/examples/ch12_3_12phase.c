
#include <avr/io.h>     //AVR 입출력에 대한 헤더 파일
#include <util/delay.h> //delay 함수사용을 위한 헤더파일

// 1-2상 여자 방식 제어 신호를 저장 하고 있는 배열
unsigned char Step[8] = {0x90, 0x80, 0xC0, 0x40, 0x60, 0x20, 0x30, 0x10};

int main(void)
{
    unsigned char i, t = 0;

    DDRB = 0x20; // MOTOR1_EN 를 출력 포트로 설정 한다.
    DDRD = 0xF0; // STEP0, STEP1, STEP2, STEP3을 출력 포트로 설정 한다.

    PORTB &= ~0x20; // M1 Disable, DC 모터 정지

    while (1)
    { // 명령당 3.75도 회전, 90도 회전하려면 총 24개의 명령이 필요
        for (i = 0; i < 24; i++)
        {
            PORTD = Step[t]; // 한 스텝 실행
            t++;             // 다음 스텝 실행을 위해 t 증가
            if (t > 7)
                t = 0; // 8 스텝을 초과하지 않도록 초기화
            _delay_ms(10);
        }
        _delay_ms(1000);
    }
}
