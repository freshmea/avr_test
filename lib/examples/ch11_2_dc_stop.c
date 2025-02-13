
#include <avr/io.h>     //AVR 입출력에 대한 헤더 파일
#include <util/delay.h> //delay 함수사용을 위한 헤더파일

#define M1_Forword 0x10
#define M1_Reverse 0x20
#define M1_Stop 0x30
#define M1_Enable 0x20

int main(void)
{
    unsigned char key = 0;

    DDRD = 0x30; // MOTOR1_IN1, MOTOR1_IN2를 출력 포트로 설정 한다.
    DDRB = 0x20; // MOTOR1_EN 를 출력 포트로 설정 한다.
    DDRE = 0x00; // 스위치 입력핀인 포트 E를 입력 포트로 설정 한다.

    // DC Motor 정회전
    PORTD = M1_Forword;
    PORTB = M1_Enable;

    while (1)
    {
        key = PINE & 0xF0; // E포트의 상위 4비트만 읽음
        if (key == 0x10)
        { //'1' 키가 눌렸을때
            // Motor1 정지, Fast Motor Stop
            PORTD |= M1_Stop;
        }
        else if (key == 0x20)
        { //'2' 키가 눌렸을때
            // Motor1 정지, Free Running Motor Stop
            PORTB &= ~M1_Enable;
        }
        else if (key == 0x40)
        { //'3' 키가 눌렸을때
            // Motor1 정회전
            PORTD = M1_Forword;
            PORTB = M1_Enable;
        }
    }
}
