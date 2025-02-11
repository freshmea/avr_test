
#include <avr/io.h>     //AVR 입출력에 대한 헤더 파일
#include <util/delay.h> //delay 함수사용을 위한 헤더파일

int main(void)
{
    unsigned char Switch_flag = 0, Switch_flag_pre = 0x01;

    DDRC = 0x0F; // 포트C 를 출력포트로 설정한다.
    DDRE = 0x00; // 포트E 를 입력포트로 설정한다.
    PORTC = 0;   // LED0 ~ 3을 OFF 한다.

    while (1)
    {
        Switch_flag = PINE >> 4; // 포트 E로 입력받은 값을 변수 Switch_flag에 저장한다.
                                 // 입력핀이 포트 E의 상위 비트이므로 우측으로 4비트 쉬프트 시킨다.

        if ((Switch_flag == 0x01) || (Switch_flag == 0x02) || (Switch_flag == 0x04) || (Switch_flag == 0x08)) // SW0 ~ 4 스위치중 하나만 눌렸을때
        {
            Switch_flag_pre = Switch_flag; // 눌린 상태 Switch_flag 값을 변수 Switch_flag_pre에 저장한다.
        }

        PORTC ^= 0x0F; // PORTC(LED가 연결된 포트)의 하위 4비트를 반전시키다.

        if (Switch_flag_pre == 0x01) // SW0을 눌렀을때
        {
            _delay_ms(125); // 125ms 시간지연
        }
        else if (Switch_flag_pre == 0x02) // SW1을 눌렀을때
        {
            _delay_ms(250); // 250ms 시간지연
        }
        else if (Switch_flag_pre == 0x04) // SW2을 눌렀을때
        {
            _delay_ms(375); // 375ms 시간지연
        }
        else if (Switch_flag_pre == 0x08) // SW3을 눌렀을때
        {
            _delay_ms(500); // 500ms 시간지연
        }
    }
}
