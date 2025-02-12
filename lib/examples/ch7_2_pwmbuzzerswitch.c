#include <avr/interrupt.h> // AVR 인터럽트에 대한 헤더파일
#include <avr/io.h>        // AVR 입출력에 대한 헤더 파일
#include <util/delay.h>    // delay 함수사용을 위한 헤더파일

// 피아노 음계에 해당하는 PWM 주파수
unsigned int DoReMi[8] = {523, 587, 659, 698, 783, 880, 987, 1046};
volatile unsigned char sound_flag = 1;
volatile unsigned char FND_DATA_TBL[] = {0x3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7C, 0X07, 0X7F, 0X67, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71, 0X08, 0X80};

// 수정: 모든 인터럽트에서 사용하도록 통일된 TCCR3A 설정 (OC3A non-inverting, Fast PWM mode14)
#define PWM_SETUP ((1 << COM3A1) | (1 << WGM31))

int main(void)
{
    DDRE = 0x08;  // 포트E PE3를 출력 나머지는 입력포트로 설정한다.
                  // Buzzer를 PE3에 연결
    DDRA = 0xFF;  // FND 출력용 PORTA를 모두 출력으로 설정
    PORTA = 0x00; // 초기 FND 초기화

    TCCR3A = PWM_SETUP;
    TCCR3B = 0x19;  // WGM3(3:2) = “11”  , CS3(2:0) = “001”  1분주 사용
    TCCR3C = 0x00;  // WGM3(3:0) = “1110”, Fast PWM, 모드 14
    TCNT3 = 0x0000; // 타이머3 카운터 초기화

    EICRB = 0xFF; // 인터럽트 4, 5, 6, 7을 상승엣지에서 동작하도록 설정한다.
    EIMSK = 0xF0; // 인터럽트 4, 5, 6, 7을 허용
    EIFR = 0xF0;  // 인터럽트 4, 5, 6, 7 플래그를 클리어

    sei();

    while (1)
    {
        if (sound_flag)
        {
            _delay_ms(2000); // 2초 지연
            TCCR3A = 0x00;   // 부저 소리를 끈다.
            sound_flag = 0;
            PORTA = 0x00; // 2초 후 FND 지우기
        }
    }
}

SIGNAL(INT4_vect) // 인터럽트 서비스 루틴
{
    cli();                       // 전체 인터럽트를 금지
    ICR3 = 14745600 / DoReMi[0]; // 도의 음향을 연주한다
    OCR3A = ICR3 / 2;
    TCCR3A = PWM_SETUP; // PE3로 출력

    sound_flag = 1;          // 부저 음이 발생하도록 설정
    PORTA = FND_DATA_TBL[0]; // INT4에서는 숫자 0 표시

    sei(); // 전체 인터럽트를 허용
}

SIGNAL(INT5_vect) // 인터럽트 서비스 루틴
{
    cli(); // 전체 인터럽트를 금지

    ICR3 = 14745600 / DoReMi[1]; // 래의 음향을 연주한다
    OCR3A = ICR3 / 2;
    TCCR3A = PWM_SETUP; // PE3로 출력

    sound_flag = 1;          // 부저 음이 발생하도록 설정
    PORTA = FND_DATA_TBL[1]; // INT5에서는 숫자 1 표시

    sei(); // 전체 인터럽트를 허용
}

SIGNAL(INT6_vect) // 인터럽트 서비스 루틴
{
    cli(); // 전체 인터럽트를 금지

    ICR3 = 14745600 / DoReMi[2]; // 미의 음향을 연주한다
    OCR3A = ICR3 / 2;
    TCCR3A = PWM_SETUP; // PE3로 출력

    sound_flag = 1;          // 부저 음이 발생하도록 설정
    PORTA = FND_DATA_TBL[2]; // INT6에서는 숫자 2 표시

    sei(); // 전체 인터럽트를 허용
}

SIGNAL(INT7_vect) // 인터럽트 서비스 루틴
{
    cli(); // 전체 인터럽트를 금지

    ICR3 = 14745600 / DoReMi[3]; // 파의 음향을 연주한다
    OCR3A = ICR3 / 2;
    TCCR3A = PWM_SETUP; // PE3로 출력

    sound_flag = 1;          // 부저 음이 발생하도록 설정
    PORTA = FND_DATA_TBL[3]; // INT7에서는 숫자 3 표시

    sei(); // 전체 인터럽트를 허용
}