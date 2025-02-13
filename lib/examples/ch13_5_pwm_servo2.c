
#include <avr/io.h>     //AVR 입출력에 대한 헤더 파일
#include <util/delay.h> //delay 함수사용을 위한 헤더파일

void Move_ServoMotor(signed char degree);

int main(void)
{
    DDRE = 0x08; // SERVO 핀을 출력 포트로 설정 한다.

    // 8분주, PWM, Phase and Frequency Correct, TOP : ICR3
    // Waveform Generation Mode : 8
    TCCR3A = 0x80; // COMnA1=1 COMnA0=0 COMnB1=0 COMnB0=0 COMnC1=0 COMnC0=0 WGMn1=0 WGMn0=0
    TCCR3B = 0x12; // ICNCn=0 ICESn=0 Blink=0 WGMn3=1 WGMn2=0 CSn2=0 CSn1=1 CSn0=0
    TCCR3C = 0x00;

    ICR3 = 18432; // frequency = 14745600/2*8*18432 = 50Hz(20ms)

    TCNT3 = 0x0000;
    OCR3A = 0x0000;

    while (1)
    {
        Move_ServoMotor(90);

        Move_ServoMotor(0);

        Move_ServoMotor(-90);
    }
}

void Move_ServoMotor(signed char degree)
{
    unsigned int _duty = 0, _pw = 0;

    //-90 = 2400
    //  0 = 1500
    //+90 =  600
    _pw = 1500 + degree * (-10);

    // 0.9216 every 1us
    _duty = 0.9216 * _pw;

    OCR3A = _duty;

    _delay_ms(1000);
}
