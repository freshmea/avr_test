#include "lcd.h"           //Text LCD를 사용하기 위한 헤더 파일
#include <avr/interrupt.h> // AVR 인터럽트에 대한 헤더파일
#include <avr/io.h>        //AVR 입출력에 대한 헤더 파일
#include <util/delay.h>    // delay 함수사용을 위한 헤더파일

#define DIR_L 0
#define DIR_R 1

unsigned char timer0Cnt = 0, mot_cnt = 0;
volatile unsigned char dir = DIR_R; // 처음방향은 우측부터
volatile unsigned char Step_flag = 0, buzzer_flag = 0, LCD_flag = 0;
// 1-2 상 여자 값을 사용
unsigned char Step[] = {0x90, 0x80, 0xC0, 0x40, 0x60, 0x20, 0x30, 0x10};
// 피아노 음계에 해당하는 PWM 주파수
unsigned int DoReMi[8] = {523, 587, 659, 698, 783, 880, 987, 1046};

int main(void)
{
    unsigned char piano = 0;

    DDRB = 0xA0; // MOTOR1_EN, PWM(OCR1C)을 출력 포트로 설정 한다.
    // PB7에 PIEZO 연결
    DDRD = 0xF0; // STEP0, STEP1, STEP2, STEP3을 출력 포트로 설정 한다.
    DDRE = 0x00; // 포트E 를 출력포트로 설정한다.

    PORTB &= ~0x20; // M1 Disable, DC 모터 정지

    lcdInit(); // Text LCD를 초기화

    TCCR0 = 0x07;
    TCNT0 = 112;    // 256-144=112 -> 10ms 마다 한번씩 인터럽트 발생
    TCCR1A |= 0x0A; // COM1C(1:0) = “10”,  OC1C핀 사용, WGM3(1:0) = “10”
    TCCR1B |= 0x19; // WGM3(3:2) = “11”  , CS3(2:0) = “001”  1분주 사용
    TCCR1C = 0x00;  // WGM3(3:0) = “1110”, Fast PWM, 모드 14
    TCNT1 = 0x0000; // 타이머1 카운터 초기화

    TIMSK = 0x01; // TOIE0 '1'
    TIFR = 0x01;  // TOV0 '1'

    EICRB = 0xFF; // 인터럽트 4, 5, 6, 7을 상승엣지에서 동작하도록 설정한다.
    EIMSK = 0xF0; // 인터럽트 4, 5, 6, 7을 허용
    EIFR = 0xF0;  // 인터럽트 4, 5, 6, 7 플래그를 클리어
    sei();

    lcdGotoXY(0, 0); // 현재 커서위치를 첫번째줄 첫번째칸으로 이동한다.
    // 첫번째 매개변수는 행을 의미하고, 두번째 매개변수는 열을 의미한다.
    lcdPrintData("STEP Motor : OFF", 16); //"STEP Motor : OFF" 문자열을 출력한다.

    lcdGotoXY(2, 1);                  // 현재 커서위치를 두번째 줄 세번째 칸으로 이동한다.
    lcdPrintData("Buzzer : OFF", 12); //"Buzzer : OFF" 문자열을 출력한다.

    while (1)
    {
        if (LCD_flag)
        {
            if (LCD_flag & 0x01) // 스텝 모터 및 부저 상태 OFF
            {
                lcdGotoXY(13, 0);
                lcdPrintData("OFF", 3); // STEP 모터 상태 off 출력
                lcdGotoXY(11, 1);
                lcdPrintData("OFF", 3); // 부저 상태 off 출력
                LCD_flag &= 0x0E;
            }
            if (LCD_flag & 0x02) // 부저 상태 ON
            {
                lcdGotoXY(11, 1);
                lcdPrintData("ON ", 3); // 부저 상태 ON 출력
                buzzer_flag = 1;        // buzzer 동작 허용
                LCD_flag &= 0x0D;
            }
            if (LCD_flag & 0x04) // 스텝 모터 상태 우측방향으로 회전(CW)
            {
                lcdGotoXY(13, 0);
                lcdPrintData("CW ", 3); // STEP 모터 상태 CW 출력
                LCD_flag &= 0x0B;
            }
            if (LCD_flag & 0x08) // 스텝 모터 상태 좌측방향으로 회전(CCW)
            {
                lcdGotoXY(13, 0);
                lcdPrintData("CCW", 3); // STEP 모터 상태 CCW 출력
                LCD_flag &= 0x07;
            }
        }
        if (buzzer_flag)
        {
            ICR1 = 14745600 / DoReMi[piano]; // 버튼에 맞는 음향을 연주한다
            OCR1C = ICR1 / 2;                // 50% 듀티비
            piano++;                         // piano 변수 1증가
            if (8 < piano)
                piano = 0; // piano가 9가 되면 초기화
            _delay_ms(1000);
        }
    }
}

SIGNAL(TIMER0_OVF_vect)
{
    cli();
    TCNT0 = 112; // 256-144=122 -> 0.01초 마다 한번씩 인터럽트 발생
    if (Step_flag)
        timer0Cnt++;    // timer0Cnt 변수를 1 증가 시킨다.
    if (timer0Cnt == 2) // 10ms * 2 = 20ms  //20ms를 얻기 위한 카운트 횟수
    {                   // (1 / (14745600 / 1024)) * 144 * 2 = 20ms -> 50hz
        timer0Cnt = 0;
        PORTD = Step[mot_cnt]; // 1-2상 여자 방식 한 스텝 진행
        if (dir == DIR_R)      // 회전 방향이 우측 방향이면
        {
            if (mot_cnt++ == 7)
                mot_cnt = 0; // 스텝 카운터 증가
        }
        else if (mot_cnt-- == 0)
            mot_cnt = 7; // 스텝 카운터 감소
    }
    sei();
}

SIGNAL(INT4_vect) // 인터럽트 서비스 루틴
{
    cli();            // 전체 인터럽트를 금지
    Step_flag = 0;    // STEP 모터 동작 정지
    PORTD = 0;        // STEP 모터 정지
    buzzer_flag = 0;  // buzzer 동작 정지
    OCR1C = 0;        // buzzer OFF
    LCD_flag |= 0x01; // LCD에 부저 및 스텝 모터 상태를 출력하도록 설정
    sei();            // 전체 인터럽트를 허용
}

SIGNAL(INT5_vect) // 인터럽트 서비스 루틴
{
    cli();            // 전체 인터럽트를 금지
    LCD_flag |= 0x02; // LCD에 부저 동작 상태를 출력하도록 설정
    sei();            // 전체 인터럽트를 허용
}

SIGNAL(INT6_vect) // 인터럽트 서비스 루틴
{
    cli();            // 전체 인터럽트를 금지
    Step_flag = 1;    // STEP 모터 동작 허용
    dir = DIR_R;      // 우측방향으로 회전
    LCD_flag |= 0x04; // LCD에 스텝모터 동작 상태를 출력하도록 설정
    sei();            // 전체 인터럽트를 허용
}

SIGNAL(INT7_vect) // 인터럽트 서비스 루틴
{
    cli(); // 전체 인터럽트를 금지

    Step_flag = 1;    // STEP 모터 동작 허용
    dir = DIR_L;      // 좌측방향으로 회전
    LCD_flag |= 0x08; // LCD에 스텝모터 동작 상태를 출력하도록 설정

    sei(); // 전체 인터럽트를 허용
}
