
#include <avr/io.h>     //AVR 입출력에 대한 헤더 파일
#include <util/delay.h> //delay 함수사용을 위한 헤더파일

int main(void)
{
    unsigned char i;

    DDRE = 0x08; // SERVO 핀을 출력 포트로 설정 한다.

    while (1)
    {
        for (i = 0; i < 50; i++) // +90도 동작
        {
            PORTE |= 0x08;
            _delay_us(600); // ON Time이 0.6[ms]
            PORTE &= ~0x08;
            _delay_us(19400); // OFF Time이 19.4[ms]
        }
        for (i = 0; i < 50; i++) // 0도 동작
        {
            PORTE |= 0x08;
            _delay_us(1500); // ON Time이 1.5[ms]
            PORTE &= ~0x08;
            _delay_us(18500); // OFF Time이 18.5[ms]
        }
        for (i = 0; i < 50; i++) // -90도 동작
        {
            PORTE |= 0x08;
            _delay_us(2400); // ON Time이 2.4[ms]
            PORTE &= ~0x08;
            _delay_us(17600); // OFF Time이 17.6[ms]
        }
    }
}
