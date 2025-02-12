
#include <avr/io.h>     // AVR 입출력에 대한 헤더 파일
#include <util/delay.h> // delay 함수사용을 위한 헤더파일

// 피아노 음계에 해당하는 PWM 주파수
unsigned int DoReMi[8] = {523, 587, 659, 698, 783, 880, 987, 1046};
unsigned int piano = 0;

int main(void)
{
    DDRB = 0x80; // 포트B PB7를 출력
                 // Buzzer를 PB7에 연결

    TCCR1A = (1 << COM1C1) | (1 << WGM11); // com1c1 이 PB7에 연결, wgm11은 fast pwm
    // com1b1 은 pb6에 연결, com1a1은 pb5에 연결
    TCCR1B = 0x19;  // WGM3(3:2) = “11”  , CS3(2:0) = “001”  1분주 사용
    TCCR1C = 0x00;  // WGM3(3:0) = “1110”, Fast PWM, 모드 14
    TCNT1 = 0x0000; // 타이머3 카운터 초기화

    while (1)
    {
        ICR1 = 14745600 / DoReMi[piano]; // 도의 음향을 연주한다
        OCR1C = ICR1 / 2;
        piano++;
        if (piano > 7)
            piano = 0;
        _delay_ms(1000); // 1초 지연
    }
}